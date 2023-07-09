#include <lib/kernel/print.h>
#include <lib/kernel/halt.h>
#include <lib/kernel/panic.h>

#include <kernel/cpu/gdt/gdt.h>

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

void callConstructors() {
  typedef void (*ConstructorFn)();
  extern ConstructorFn start_ctors;
  extern ConstructorFn end_ctors;

  for(ConstructorFn* i = &start_ctors;i != &end_ctors; i++) {
    (*i)();
  }
}

extern "C" void kmain(void) {

  callConstructors();

  Framebuffer::clearScreen();
  Kernel::println("Hello there, {}", 42);
  Kernel::println("The quick brown fox jumped over the lazy cat");

  GDT::initialize();

#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
