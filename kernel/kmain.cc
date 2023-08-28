#include <kernel/drivers/ps2.h>
#include <kernel/drivers/serial.h>
#include <kernel/arch/platform.h>
#include <kernel/utils/halt.h>
#include <kernel/graphics/framebuffer.h>
#include <kernel/memory/memory.h>

#include <lib/libcxx.h>

#define LOG_LEVEL 1

#include <kernel/utils/logger.h>

#ifdef ENABLE_TESTS
#include "tests/tester.h"
#endif

extern "C" auto kmain(void) -> void {
  serial::initialize();
  LibCXX::callGlobalConstructors();

  Arch::initialize();

  Framebuffer::initialize();

  PS2::Keyboard::initialize();
  Memory::initialize();

#ifdef ENABLE_TESTS
  Tester::main();
#endif
  Log::debug("this shall not be shown!");

  Kernel::halt();
}
