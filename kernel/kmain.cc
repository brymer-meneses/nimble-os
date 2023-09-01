#include <kernel/drivers/ps2.h>
#include <kernel/drivers/serial.h>
#include <kernel/arch/arch.h>
#include <kernel/utils/halt.h>
#include <kernel/graphics/framebuffer.h>
#include <kernel/memory/memory.h>

#include <lib/libcxx.h>

#define LOG_LEVEL 0

#include <kernel/utils/logger.h>

#ifdef ENABLE_TESTS
#include "tests/tester.h"
#endif

extern "C" auto kmain(void) -> void {
  serial::initialize();
  libcxx::callGlobalConstructors();

  arch::initialize();

  framebuffer::initialize();

  ps2::keyboard::initialize();
  memory::initialize();

#ifdef ENABLE_TESTS
  tester::main();
#endif

  kernel::halt();
}
