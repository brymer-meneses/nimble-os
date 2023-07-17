#include <lib/thirdparty/limine.h>
#include <lib/types.h>

#include <kernel/acpi/acpi.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

static volatile limine_rsdp_request rsdpRequest = {
  .id = LIMINE_RSDP_REQUEST, 
  .revision = 0
};

struct RSDP {
  char signature[8];
  u8 checksum;
  char OEMID[6];
  u8 revision;
  u32 rsdtAddress;
  u32 length;
  u64 xsdtAddress;
  u8 extendedChecksum;
  u8 reserved[3];
} __attribute__((packed));

using ACPI::AcpiHeader;

static RSDP* rsdp = nullptr;
static AcpiHeader* rsdt = nullptr;

constexpr u16 RSDP_VERSION_TWO_SIZE = sizeof(RSDP);
// same as version two, except we remove the last four fields
constexpr u16 RSDP_VERSION_ONE_SIZE = sizeof(RSDP) -  sizeof(u32) - sizeof(u64) - 4*sizeof(u8);

[[nodiscard]]
auto usesVersionTwo() -> bool {
   return rsdp->revision >= 2 &&  rsdp->xsdtAddress != 0;
}

[[nodiscard]]
auto validateChecksum(u8* byteArray, size_t size) -> bool {
  u32 checksum = 0;
  for (size_t i = 0; i < size; i++) {
    checksum += byteArray[i];
  }

  return (checksum & 0xFF) == 0;
}

void ACPI::initialize() { 
  
  limine_rsdp_response* rsdpResponse = rsdpRequest.response;

  if (rsdpResponse == nullptr || rsdpResponse->address == nullptr) {
    Kernel::panic("ACPI not supported.");
  }

  rsdp = (RSDP*) rsdpResponse->address;

  if (usesVersionTwo()) {
    rsdt = (AcpiHeader*) (rsdp->xsdtAddress);
  } else {
    rsdt = (AcpiHeader*) ((u64) rsdp->rsdtAddress);
  }
  
  bool isChecksumValid =  validateChecksum((u8*) rsdp, usesVersionTwo() ? RSDP_VERSION_TWO_SIZE : RSDP_VERSION_ONE_SIZE);


  if (!isChecksumValid) {
    Kernel::panic("Failed to validate RSDP");
  }
  Kernel::println("ACPI {}", rsdpResponse->revision);
  Kernel::println("Uses XSDT {}", usesVersionTwo() ? "true" : "false");
  Kernel::println("Address {hex}", (uintptr_t) rsdt);
};

[[nodiscard]]
auto ACPI::findAcpiHeader(const char signature[4]) -> std::optional<AcpiHeader*> {
  // if we are using the xsdt, each `sdt` it points to has a size of u64, otherwise 
  // we it should have a size of u32
  const u8 sdtSize = usesVersionTwo() ? sizeof(u64) : sizeof(u32);

  size_t totalItems = (rsdt->length - sizeof(AcpiHeader)) / sdtSize;

  // I kept stumbling at this stupid bug wherein I was using `rsdp` below
  // instead of `rsdt`
  u8* address = (u8*) rsdt;
  address += sizeof(AcpiHeader);

  for (size_t i=0; i<totalItems; i++) {

    AcpiHeader* sdt = nullptr;
    if (usesVersionTwo()) {
      sdt = (AcpiHeader*) (uintptr_t) (*(u64*) address);
    } else {
      sdt = (AcpiHeader*) (uintptr_t) (*(u32*) address);
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

[[nodiscard]]
auto ACPI::isInitialized() -> bool {
  return rsdp != nullptr;
}
