#include "apic.h"
#include <kernel/acpi/madt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
#include <kernel/drivers/pic.h>
#include <kernel/drivers/apic/lapic.h>
#include <kernel/drivers/apic/ioapic.h>

void APIC::initialize() {

  auto [ioApics, ioApicsCount] = ACPI::MADT::getIoApics();

  KERNEL_ASSERT(ioApicsCount != 0);

  // initialize the PIC make sure it is remapped and not touch the exception
  // vectors
  PIC::initialize();
  // mask all the interrupts since we're using the APIC
  PIC::maskAll();

  LAPIC::initialize();
}

