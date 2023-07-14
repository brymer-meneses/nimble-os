
#include "acpi.h"
#include "madt.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

using ACPI::AcpiHeader;
using ACPI::MADT::IoApic;
using ACPI::MADT::IsoApic;

struct MadtRecord {
  u8 type;
  u8 length;
} __attribute__((packed));

struct MadtHeader {
  AcpiHeader header;
  u32 localApicAddress;
  u32 flags;
} __attribute__((packed));

constexpr u8 LOCAL_APIC = 0;
constexpr u8 IO_APIC = 1;
constexpr u8 ISO = 2;
constexpr u8 NMI = 4;

// TODO: 
// I currently have 5 in qemu but should use a dynamic array to store this.
// Will do this after I figure out memory allocation.
static IsoApic* ISO_APIC_TABLE[5];

static IoApic* gIoApic = nullptr;

auto ACPI::MADT::getIoApic() -> std::optional<IoApic*> {
  if (gIoApic == nullptr) {
    return std::nullopt;
  }

  return gIoApic;
}

void ACPI::MADT::initialize() {
  std::optional<AcpiHeader*> madtHeader = ACPI::findAcpiHeader("APIC");
  if (!madtHeader) {
    Kernel::panic("Cannot find APIC");
  };

  MadtHeader* madt = (MadtHeader*) madtHeader.value();

  Kernel::println("Found APIC at {hex}", (u32) madt->localApicAddress);

  // the madt record is found after the madt header
  MadtRecord* madtRecord = (MadtRecord*) ((u8*) madt + sizeof(MadtHeader));

  size_t length = (madt->header.length -  sizeof(MadtHeader));

  u8* address = (u8*) madtRecord;

  u8 isoApicCount = 0;

  while (length > 0) {
    MadtRecord* record = (MadtRecord*) address;

    switch (record->type) {
      case LOCAL_APIC: {
        LocalApic* lapic = (LocalApic*) address;
        Kernel::println("Found LAPIC type {}", lapic->type);
      } break;
      case IO_APIC: {
        gIoApic = (IoApic*) address;
        Kernel::println("Found IO APIC address base {hex}", gIoApic->addressBase);
      } break;
      case ISO: {
        IsoApic* isoApic =  (IsoApic*) address;
        ISO_APIC_TABLE[isoApicCount] = isoApic;
        Kernel::println("Found ISO APIC irq source {bin}", isoApic->flags);
      } break;
      case NMI: {
        NmiApic* nmi = (NmiApic*) address;
        Kernel::println("Found NMI APIC type {}", nmi->type);
      } break; 
      default: {
        Kernel::panic("Unknown record type {}", record->type);
      } break;
    }

    length -= record->length;
    address += record->length;
  }
}

