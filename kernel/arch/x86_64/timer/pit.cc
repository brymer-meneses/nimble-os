#include <kernel/utils/logger.h>
#include <kernel/utils/print.h>
#include <kernel/arch/x86_64/interrupt/pic.h>
#include <kernel/arch/x86_64/interrupt/interrupt.h>
#include <kernel/drivers/io.h>

#include "pit.h"

using namespace x86_64;

static constexpr u8 PIC_COMMAND = 0x43;
static constexpr u8 PIC_CHANNEL0 = 0x40;

static auto timerCallback() -> void {
  kernel::print(".");
  PIC::sendEndOfInterrupt(32);
};

auto PIT::initialize() -> void {
  IO::outb(PIC_COMMAND, 0b00110100);

  interrupt::setIrqHandler(32, timerCallback);
  PIC::clearMask(0);

  log::info("Initialized PIT");
}

auto PIT::setPeriodic(u16 count) -> void {
  IO::outb(PIC_CHANNEL0, count & 0xFF);
  IO::outb(PIC_CHANNEL0, (count >> 8) & 0xFF);
}

