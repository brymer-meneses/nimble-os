#pragma once

#include <kernel/drivers/framebuffer.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/format.h>
#include <lib/types.h>
#include "halt.h"

namespace Kernel {

  [[noreturn]] inline auto panic(const char* string) -> void {
    Framebuffer::setForeground(0xBF616A);
    Framebuffer::writeString("[Kernel Panic]: ");
    Framebuffer::writeString(string);
    Framebuffer::writeCharacter('\n');
    Kernel::halt();
  }

  template<typename Arg, typename ...Args>
  [[noreturn]] auto panic(const char* string, Arg arg, Args ...args) -> void {
    char buffer[256];
    Format::format(buffer, string, arg, args...);
    panic(buffer);
  }

}
