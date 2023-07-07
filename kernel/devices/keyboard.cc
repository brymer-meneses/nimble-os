#include "keyboard.h"
#include "interrupt/interrupt.h"
#include "vga/vga.h"
#include "drivers/pic.h"

void irq_handler() {
  pic::finish_interrupt(1);
  vga::println("Interrupt called");
}

void keyboard::initialize() {
  pic::clear_mask(1);
}


