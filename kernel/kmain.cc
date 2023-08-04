#include <kernel/utils/halt.h>

#include <kernel/drivers/ps2.h>
#include <kernel/drivers/framebuffer.h>

#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>

#include "kernel/arch/prelude.h"

#include <lib/ctors.h>

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

extern "C" auto kmain(void) -> void {

  callGlobalConstructors();

  Arch::initialize();

  Framebuffer::clearScreen();

  PS2::Keyboard::initialize();
  PMM::initialize();
  VMM::initialize();

#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
