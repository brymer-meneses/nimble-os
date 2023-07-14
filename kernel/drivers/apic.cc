#include "apic.h"
#include <kernel/acpi/madt.h>
#include <kernel/utils/panic.h>

using ACPI::MADT::IoApic;

void APIC::initialize() {
  std::optional<IoApic*> ioApic = ACPI::MADT::getIoApic();

  if (!ioApic) {
    Kernel::panic("IOAPIC is not initialized");
  }
}

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

auto IOAPIC::read(IoApic* ioApic, u32 reg) -> u32 {
  u64 base = ioApic->addressBase;
  // the IOREGSL where we want to write to
  *(volatile u32*) base = reg;
  // write the value to IOWIN
  return *(u32*)(base + 0x10);
}

auto IOAPIC::write(IoApic* ioApic, u32 reg, u32 value) -> u32 {
  u64 base = ioApic->addressBase;
  // the IOREGSL where we want to read from
  *(volatile u32*) (base + 0x0) = reg;
  // return the from the IOWIN
  return *(u32*)(base + 0x10) = value;
}
