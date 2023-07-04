#include "vga/vga.h"
#include "interrupt/interrupt.h"

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

inline void halt() {
  asm("cli");
  asm("hlt");
}


extern "C" void kmain(void) {
  vga::clear_screen();

  interrupt::initialize_idt();

#ifdef ENABLE_TESTS
  tester::main();
#endif


  while (true) {
    halt();
  }
}
