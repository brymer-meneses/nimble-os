#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/macros.h>
#include <kernel/utils/assert.h>

#include <kernel/cpu/gdt/gdt.h>
#include <kernel/cpu/interrupt/interrupt.h>
#include <kernel/cpu/interrupt/idt.h>
#include <kernel/acpi/acpi.h>
#include <kernel/acpi/madt.h>
#include <kernel/drivers/apic/apic.h>
#include <kernel/drivers/ps2.h>

#include <kernel/memory/pmm.h>

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

auto callConstructors() -> void {
  using ConstructorFn = void(*)();
  extern ConstructorFn start_ctors;
  extern ConstructorFn end_ctors;

  for(ConstructorFn* i = &start_ctors;i != &end_ctors; i++) {
    (*i)();
  }
}


extern "C" auto kmain(void) -> void {

  callConstructors();

  Framebuffer::clearScreen();

  GDT::initialize();
  IDT::initialize();
  Interrupt::initialize();

  // ACPI::initialize();
  // ACPI::MADT::initialize();

  // APIC::initialize();
  PS2::Keyboard::initialize();
  PMM::initialize();

#ifdef ENABLE_TESTS
  Tester::main();
#endif

  Kernel::halt();
}
