#include "vga/vga.h"
#include "interrupt/interrupt.h"

inline void halt() {
  asm("cli");
  asm("hlt");
}

extern "C" void kmain(void) {
  vga::clear_screen();
  vga::println("Hello world!");

  interrupt::initialize_idt();

  while (true) {
    halt();
  }
}
