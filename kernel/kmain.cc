#include <lib/kernel/print.h>
#include <lib/kernel/halt.h>
#include <lib/kernel/panic.h>
#include <lib/macros.h>

#include <kernel/cpu/gdt/gdt.h>
#include <kernel/cpu/interrupt/interrupt.h>
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



extern "C" void kmain(void) {

  callConstructors();

  Framebuffer::clearScreen();

  INITIALIZE(GDT);
  INITIALIZE(IDT);
  INITIALIZE(Interrupt);

  Kernel::println("Hi {}", 12345);

  asm volatile ("int 0x1");


#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
