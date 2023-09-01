#include "apic.h"
#include <kernel/acpi/madt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
// #include <kernel/drivers/pic.h>
#include "lapic.h"

void APIC::initialize() {

  auto [ioApics, ioApicsCount] = ACPI::MADT::getIoApics();

  kernel::assert(ioApicsCount != 0);

  // initialize the PIC make sure it is remapped and not touch the exception
  // vectors
  // PIC::initialize();
  // mask all the interrupts since we're using the APIC
  // PIC::maskAll();

  LAPIC::initialize();
}

