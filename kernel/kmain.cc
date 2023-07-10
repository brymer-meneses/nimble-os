#include <lib/kernel/print.h>
#include <lib/kernel/halt.h>
#include <lib/kernel/panic.h>

#include <kernel/cpu/gdt/gdt.h>
#include <kernel/cpu/interrupt/idt.h>

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

#define INITIALIZE(module) \
  Kernel::print("Initializing " #module "..."); \
  module::initialize(); \
  Kernel::println("  [Done]");

extern "C" void kmain(void) {

  callConstructors();

  Framebuffer::clearScreen();
  Kernel::println("Hello kernel!");

  // INITIALIZE(GDT);
  INITIALIZE(IDT);

  asm volatile ("int 0x00");


#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
