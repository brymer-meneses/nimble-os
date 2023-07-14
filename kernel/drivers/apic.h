#pragma once

#include <kernel/acpi/madt.h>


namespace APIC {

  void initialize();


}

namespace IOAPIC {
  using ACPI::MADT::IoApic;

  void write(IoApic* ioApic, u32 reg);
  [[nodiscard]] auto read(IoApic* ioApic, u32 reg) -> u32;
  [[nodiscard]] auto write(IoApic* ioApic, u32 reg, u32 value) -> u32;
}
