#include <lib/kernel.h>
#include <lib/color.h>

extern "C" void kmain(void) {

  Framebuffer::clearScreen();
  Kernel::print("Hello there, {}\n", 42);
  Kernel::print("The quick brown fox jumped over the lazy cat");


  Kernel::halt();
}
