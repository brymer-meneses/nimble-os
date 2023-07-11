#pragma once

#include <kernel/drivers/framebuffer.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/format.h>
#include "halt.h"

namespace Kernel {

  template<typename Arg, typename ...Args>
  void panic(const char* string, Arg arg, Args ...args) {
    char buffer[256];
    Format::format(buffer, string, arg, args...);

    Framebuffer::setForeground(0xBF616A);
    Framebuffer::writeString("[Kernel Panic]: ");
    Framebuffer::writeString(buffer);
    Framebuffer::writeCharacter('\n');
    Kernel::halt();
  }

  inline void panic(const char* string) {
    Framebuffer::setForeground(0xBF616A);
    Framebuffer::writeString("[Kernel Panic]: ");
    Framebuffer::writeString(string);
    Framebuffer::writeCharacter('\n');
    Kernel::halt();
  }
}
