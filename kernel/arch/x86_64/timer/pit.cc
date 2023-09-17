#include <kernel/utils/logger.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <kernel/arch/x86_64/interrupt/pic.h>
#include <kernel/arch/x86_64/interrupt/interrupt.h>
#include <kernel/drivers/io.h>

#include "pit.h"

using namespace x86_64;

static constexpr u8 PIC_COMMAND = 0x43;
static constexpr u8 PIC_DATA = 0x40;
static constexpr u16 PIC_PERIOD = 838;

auto PIT::setPeriodic(u64 count) -> void {
  IO::outb(PIC_DATA, count & 0xFF);
  IO::outb(PIC_DATA, (count >> 8) & 0xFF);
  log::debug("PIT: set periodic {}", count);
}

auto PIT::read() -> u64 {
  IO::outb(PIC_COMMAND, 0);

  u16 count = 0;
  count = IO::inb(PIC_DATA);
  count |= (u16) (IO::inb(PIC_DATA) << 8);
  return count;
}

auto PIT::wait(u64 nanos) -> void {
  kernel::assert(nanos / PIC_PERIOD < 0xFFFF, "Sleep time too long for PIT");

  const u16 target = 0xFFFF - (nanos / PIC_PERIOD);

  IO::outb(PIC_COMMAND, 0x34);
  IO::outb(PIC_DATA, 0xFF);
  IO::outb(PIC_DATA, 0xFF);

  while (PIT::read() > target) { 
    kernel::halt();
  }

}
