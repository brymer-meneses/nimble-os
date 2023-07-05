#include "vga/vga.h"
#include "interrupt/interrupt.h"
#include "lib/serial.h"

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

inline void halt() {
  asm("cli");
  asm("hlt");
}


extern "C" void kmain(void) {
  vga::clear_screen();
  vga::println("Hello, {s}!", "world");

  interrupt::initialize_idt();
  serial::initialize();

  serial::println("Hello {s}!", "serial");


#ifdef ENABLE_TESTS
  tester::main();
#endif


  while (true) {
    halt();
  }
}
