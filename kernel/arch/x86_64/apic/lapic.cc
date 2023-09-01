#include "lapic.h"
#include <kernel/acpi/madt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <kernel/drivers/io.h>

// values can be found at intel/amd software development manual
#define LAPIC_SPURIOUS_REG    0xF0
#define LAPIC_EOI_REG         0xB0
#define LAPIC_TIMER_LTV       0x320
#define LAPIC_APIC_ID         0x20

static uintptr_t localApicAddress = 0;

static void lapicWrite(u32 reg, u32 value) {
  MMIO::write<u32>(localApicAddress + reg, value);
}

static auto lapicRead(u32 reg) -> u32 {
  return MMIO::read<u32>(localApicAddress + reg);
}

void LAPIC::initialize() {
  localApicAddress = ACPI::MADT::getLocalApicAddress();

  kernel::assert(localApicAddress);

  lapicWrite(LAPIC_SPURIOUS_REG, lapicRead(LAPIC_SPURIOUS_REG) | (1<<8) | 0xff);
}

