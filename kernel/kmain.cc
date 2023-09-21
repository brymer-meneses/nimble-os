#include <kernel/drivers/ps2.h>
#include <kernel/drivers/serial.h>
#include <kernel/arch/arch.h>
#include <kernel/utils/halt.h>
#include <kernel/graphics/framebuffer.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/pmm.h>
#include <kernel/tasking/scheduler.h>

#include <kernel/arch/timer.h>

#include <lib/libcxx.h>

#define LOG_LEVEL 0

#include <kernel/utils/logger.h>
#include <kernel/utils/assert.h>

using process::Process;

auto kmain() -> void;

extern "C" auto init() -> void {
  serial::initialize();
  libcxx::callGlobalConstructors();

  arch::initialize();

  framebuffer::initialize();

  memory::initialize();
  ps2::keyboard::initialize();

  scheduler::addProcess(new Process("kmain", kmain, false));
  scheduler::initialize();
}

auto kmain() -> void {
  log::info("entering kmain");
  

  process::exit(0);

}
