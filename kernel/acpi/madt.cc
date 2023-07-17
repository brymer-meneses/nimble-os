
#include "acpi.h"
#include "madt.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

using ACPI::AcpiHeader;
using ACPI::MADT::IoApic;
using ACPI::MADT::NonMaskableInterrupt;
using ACPI::MADT::InterruptSourceOverride;
using ACPI::MADT::LocalApic;

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

// NOTE:
// this is ugly and highly memory unsafe, will revisit once i get `malloc`
// working
static IoApic* ioApics[16];
static u8 ioApicCount = 0;

auto ACPI::MADT::getIoApics() -> std::pair<IoApic**, size_t> {
  return std::make_pair(ioApics, ioApicCount);
}

static LocalApic* localApics[16];
static u8 localApicCount = 0;

auto ACPI::MADT::getLocalApics() -> std::pair<LocalApic**, size_t> {
  return std::make_pair(localApics, localApicCount);
}

static NonMaskableInterrupt* nonMaskableInterrupts[16];
static u8 nonMaskableInterruptCount = 0;

auto ACPI::MADT::getNonMaskableInterrupts() -> std::pair<NonMaskableInterrupt**, size_t> {
  return std::make_pair(nonMaskableInterrupts, nonMaskableInterruptCount);
}

static InterruptSourceOverride* interruptSourceOverrides[16];
static u8 interruptSourceOverrideCount = 0;

auto ACPI::MADT::getInterruptSourceOverrides() -> std::pair<InterruptSourceOverride**, size_t> {
  return std::make_pair(interruptSourceOverrides, interruptSourceOverrideCount);
}

static uintptr_t gLocalApicAddress = 0;

auto ACPI::MADT::getLocalApicAddress() -> uintptr_t {
  return gLocalApicAddress;
}



void ACPI::MADT::initialize() {
  std::optional<AcpiHeader*> madtHeader = ACPI::findAcpiHeader("APIC");
  if (!madtHeader) {
    Kernel::panic("Cannot find APIC");
  };

  MadtHeader* madt = (MadtHeader*) madtHeader.value();

  gLocalApicAddress = madt->localApicAddress;
  Kernel::println("Found Local APIC at {hex}",  gLocalApicAddress);

  // the madt record is found after the madt header
  MadtRecord* madtRecord = (MadtRecord*) ((u8*) madt + sizeof(MadtHeader));

  size_t length = (madt->header.length -  sizeof(MadtHeader));

  u8* address = (u8*) madtRecord;

  // u8 isoApicCount = 0;

  while (length > 0) {
    MadtRecord* record = (MadtRecord*) address;

    switch (record->type) {
      case LOCAL_APIC: {
        localApics[localApicCount++] = (LocalApic*) record;
      } break;
      case IO_APIC: {
        ioApics[ioApicCount++] = (IoApic*) record;
      } break;
      case ISO: {
        interruptSourceOverrides[interruptSourceOverrideCount++] = (InterruptSourceOverride*) record;
      } break;
      case NMI: {
        nonMaskableInterrupts[nonMaskableInterruptCount++] = (NonMaskableInterrupt*) record;
      } break; 
      default: {
        Kernel::panic("Unknown record type {}", record->type);
      } break;
    }

    length -= record->length;
    address += record->length;
  }
}

