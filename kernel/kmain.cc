#include "vga/vga.h"
#include "interrupt/interrupt.h"
#include "drivers/pic.h"
#include "../arch/prelude.h"
#include "drivers/serial.h"
#include "devices/keyboard.h"

#ifdef ENABLE_TESTS
  #include "tests/tester.h"
#endif

inline void halt() {
  asm volatile ("hlt");
}


extern "C" void kmain(void) {
  vga::clear_screen();

  interrupt::initialize_idt();
  pic::initialize();

  serial::initialize();
  vga::println("Hello, {s}!", "world");
  serial::println("Hello {s}!", "serial");


  keyboard::initialize();


#ifdef ENABLE_TESTS
  tester::main();
#endif


  while (true) {
    asm volatile ("hlt");
  }
}
