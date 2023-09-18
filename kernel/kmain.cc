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

#ifdef ENABLE_TESTS
#include "tests/tester.h"
#endif

auto helloWorld() -> void {
  serial::println("hello scheduler!");
  process::exit(0);
}

extern "C" auto kmain(void) -> void {
  serial::initialize();

  libcxx::callGlobalConstructors();

  arch::initialize();

  framebuffer::initialize();

  ps2::keyboard::initialize();
  memory::initialize();

  auto* process = new process::Process("helloWorld", helloWorld, false);
  scheduler::addProcess(process);
  scheduler::initialize();


#ifdef ENABLE_TESTS
  tester::main();
#endif

  kernel::halt();
}
