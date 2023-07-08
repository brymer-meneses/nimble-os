#include <lib/kernel.h>
#include <lib/color.h>
#include <drivers/framebuffer.h>

extern "C" void kmain(void) {

  Framebuffer::clearScreen();

  Framebuffer::writeString("Hello Kernel!\n");
  Framebuffer::writeString("Hello Kernel!\n");


  Kernel::halt();
}
