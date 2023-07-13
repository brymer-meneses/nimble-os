#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/macros.h>
#include <kernel/utils/assert.h>

#include <kernel/cpu/gdt/gdt.h>
#include <kernel/cpu/interrupt/interrupt.h>
#include <kernel/cpu/interrupt/idt.h>
#include <kernel/acpi/acpi.h>


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

  GDT::initialize();
  IDT::initialize();
  Interrupt::initialize();
  ACPI::initialize();

  asm volatile ("int 0x0");

#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
