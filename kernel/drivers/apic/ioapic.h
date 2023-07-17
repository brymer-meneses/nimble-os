#pragma once
#include <kernel/acpi/madt.h>

namespace IOAPIC {

  void setRedirect(u32 lapicId, u8 vector, u8 irq, bool status);
  

}
