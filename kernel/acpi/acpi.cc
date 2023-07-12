#include <lib/thirdparty/limine.h>

#include <kernel/acpi/acpi.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

#include <cstddef>

static volatile limine_rsdp_request rsdpRequest = {
  .id = LIMINE_RSDP_REQUEST, 
  .revision = 0
};

struct RSDP {
  char signature[8];
  uint8_t checksum;
  char OEMID[6];
  uint8_t revision;
  uint32_t rsdtAddress;
  uint32_t length;
  uint64_t xsdtAddress;
  uint8_t extendedChecksum;
  uint8_t reserved[3];
} __attribute__((packed));

using ACPI::SDT;

static RSDP* rsdp = nullptr;
static SDT* rsdt = nullptr;

[[nodiscard]]
bool validateChecksum(uint8_t* byteArray, size_t size) {
  uint32_t checksum = 0;
  for (size_t i = 0; i < size; i++) {
    checksum += byteArray[i];
  }

  return (checksum & 0xFF) == 0;
}


[[nodiscard]]
bool usesVersionTwo() {
   return rsdp->revision >= 2 &&  rsdp->xsdtAddress != 0;
}

constexpr uint16_t RSDP_VERSION_TWO_SIZE = sizeof(RSDP);

// same as version two, except we remove the last four fields
constexpr uint16_t RSDP_VERSION_ONE_SIZE = sizeof(RSDP) -  sizeof(uint32_t) - sizeof(uint64_t) - 4*sizeof(uint8_t);

void ACPI::initialize() { 
  
  limine_rsdp_response* rsdpResponse = rsdpRequest.response;

  if (rsdpResponse == nullptr || rsdpResponse->address == nullptr) {
    Kernel::panic("ACPI not supported.");
  }

  rsdp = (RSDP*) rsdpResponse->address;

  if (usesVersionTwo()) {
    rsdt = (SDT*) (rsdp->xsdtAddress);
  } else {
    rsdt = (SDT*) ((uint64_t) rsdp->rsdtAddress);
  }
  
  bool isChecksumValid =  validateChecksum((uint8_t*) rsdp, usesVersionTwo() ? RSDP_VERSION_TWO_SIZE : RSDP_VERSION_ONE_SIZE);


  if (!isChecksumValid) {
    Kernel::panic("Failed to validate RSDP");
  }
  Kernel::println("ACPI {}", rsdpResponse->revision);
  Kernel::println("Uses XSDT {}", usesVersionTwo() ? "true" : "false");
  Kernel::println("Address {hex}", (uintptr_t) rsdt);
};

[[nodiscard]]
auto ACPI::findSDT(const char signature[4]) -> std::optional<SDT*> {
  // if we are using the xsdt, each `sdt` it points to has a size of 64 bytes, otherwise 
  // we use 32 bytes
  const uint8_t sdtSize = usesVersionTwo() ? sizeof(uint64_t) : sizeof(uint32_t);

  size_t totalItems = (rsdt->length - sizeof(SDT)) / sdtSize;

  // I kept stumbling at this stupid bug wherein I was using `rsdp` below
  // instead of `rsdt`
  uint8_t* address = (uint8_t*) rsdt;
  address += sizeof(SDT);

  for (size_t i=0; i<totalItems; i++) {

    SDT* sdt = nullptr;
    if (usesVersionTwo()) {
      sdt = (SDT*) (uintptr_t) (*(uint64_t*) address);
    } else {
      sdt = (SDT*) (uintptr_t) (*(uint32_t*) address);
    }

    if (sdt == nullptr) continue;

    // the signature is a char array that is not null-terminated
    if (std::memcmp(signature, sdt->signature, 4) == 0) {
      return sdt;
    }

    address += sdtSize;
  }

  return std::nullopt;
}
