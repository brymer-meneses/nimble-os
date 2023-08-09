#include <kernel/utils/halt.h>
#include <kernel/drivers/ps2.h>
#include <kernel/graphics/framebuffer.h>
#include <kernel/memory/memory.h>
#include <kernel/arch/platform.h>

#include <lib/libcxx.h>

#ifdef ENABLE_TESTS
#include "tests/tester.h"
#endif

extern "C" auto kmain(void) -> void {

  Arch::initialize();

  LibCXX::callGlobalConstructors();

  Framebuffer::clearScreen();

  PS2::Keyboard::initialize();
  Memory::initialize();

#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
