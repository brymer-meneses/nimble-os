#include "ioapic.h"
#include <cstddef>
#include <kernel/drivers/io.h>
#include <kernel/acpi/madt.h>
#include <kernel/utils/assert.h>
#include <kernel/utils/panic.h>

// NOTES:
//
// The IO-APIC has two memory mapped registers.
//
// | Memory Address | Mnemonic Name | Register Name        | Description                              |
// | -------------- | ------------- | -------------------- | ---------------------------------------- |
// | FEC0 0000h     | IOREGSEL      | I/O Registere Select | Used to select the IO Register to access |
// | FEC0 0010h     | IOWIN         | I/O Window Data      | Used to access data selected by IOREGSEL |
//
// The memory address here is generally where it's located, but not always the case
//
// Notice that IOWIN = IOREGSEL + 0x10
// Using the `IOREGSEL`, we can access the following registers:
//
// | Name       | Offset  | Description                                             |
// | ---------- | ------- | ------------------------------------------------------- |
// | IOAPICID   | 00h     | Identification register for the IOAPIC                  |
// | IOAPICOVER | 01h     | IO APIC Version                                         |
// | IOAPICAARB | 02h     | It contains the BUS arbitration priority for the IOAPIC |
// | IOREDTBL   | 03h-3fh | The redirection tables (see the IOREDTBL Paragraph)     |

using ACPI::MADT::IoApic;

static auto read(IoApic* ioApic, u32 reg) -> u32 {
  IO::Mem::write<u32>(ioApic->addressBase, reg);
  return IO::Mem::read<u32>(ioApic->addressBase + 0x10);
}

static auto write(IoApic* ioApic, u32 reg, u32 value) -> void {
  IO::Mem::write<u32>(ioApic->addressBase, reg);
  IO::Mem::write<u32>(ioApic->addressBase + 0x10, value);
}

static auto getIoApicEntries(IoApic* ioApic) -> size_t {
  return (read(ioApic, 0x1) & 0xff0000) >> 16;
}

static auto findIoApicWithGsi(u32 gsi) -> IoApic* {
  auto [ioApics, count] = ACPI::MADT::getIoApics();

  for (size_t i=0; i<count; i++) {
    IoApic* current = ioApics[i];
    bool isIoApicInRangeOfGsi = gsi >= current->globalInterruptBase && gsi < current->globalInterruptBase + getIoApicEntries(current);
    if (isIoApicInRangeOfGsi) 
      return current;
  };

  Kernel::panic("Cannot find IOAPIC that handles the GSI: {}", gsi);
  __builtin_unreachable();
}

static auto setGsiRedirect(u32 lapicId, u8 vector, u8 gsi, u16 isoFlags, bool status) -> void {
  // get the IoApic that is responsible for the GSI
  IoApic* ioApic = findIoApicWithGsi(gsi);



  // NOTE:
  // the `flags` come from the ISO which is `0` if the gsi does not come from an
  // interrupt source override
  //
  // it has the following structure:
  //
  // Polarity
  // Length: 2 bits, Offset: 0 of the APIC/IO input signals
  //  - 00 Use the default settings is active-low for level-triggered interrupts)
  //  - 01 Active High
  //  - 10 Reserved
  //  - 11 Active Low
  //
  // Trigger Mode
  //   (Length: 2 bits, Offset: 2)
  //  - 00 Use the default settiungs (in the ISA is edge-triggered)
  //  - 01 Edge-triggered
  //  - 10 Reserved
  //  - 11 Level-Triggered
  //
  u64 redirect = vector;
  if ((isoFlags & (1 << 1)) != 0) {
    redirect |= (1 << 13);
  }

  if ((isoFlags & (1 << 3)) != 0) {
    redirect |= (1 << 15);
  }

  if (!status) {
    redirect |= (1 << 16);
  }

  redirect |= (u64) lapicId << 56;
  u32 redirectEntry = (gsi - ioApic->globalInterruptBase) * 2 + 16;

  Kernel::println("ioredtbl entry {bin}", redirect);
  Kernel::println("redirection entry {}", redirectEntry);

  write(ioApic, redirectEntry,     (u32) (redirect));
  write(ioApic, redirectEntry + 1, (u32) (redirect >> 32));
}

auto IOAPIC::setRedirect(u32 lapicId, u8 vector, u8 irq, bool status) -> void {

  // first check if there's a set handler for the irq in the interrupt source
  // overrides

  auto [interruptSourceOverrides, count] = ACPI::MADT::getInterruptSourceOverrides();

  for (size_t i=0; i<count; i++) {
    if (interruptSourceOverrides[i]->irqSource != irq)
      continue;

    // now we found the iso
    setGsiRedirect(lapicId, vector,
                   interruptSourceOverrides[i]->globalInterrupt,
                   interruptSourceOverrides[i]->flags, status);
    return;
  }

  // 0 is the gsi base of the default ioapic
  setGsiRedirect(lapicId, vector, irq, 0, status);
}
