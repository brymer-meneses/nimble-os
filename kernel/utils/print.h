#pragma once
#include "../lib/format.h"
#include "../kernel/drivers/framebuffer.h"

namespace Kernel {
  template<typename Arg, typename ...Args>
  void print(const char* string, Arg arg, Args ...args) {
    char buffer[256];
    Format::format(buffer, string, arg, args...);
    Framebuffer::writeString(buffer);
  }

  template<typename Arg, typename ...Args>
  void println(const char* string, Arg arg, Args ...args) {
    char buffer[256];
    Format::format(buffer, string, arg, args...);
    Framebuffer::writeString(buffer);
    Framebuffer::writeCharacter('\n');
  }

  inline void print(const char* string) {
    Framebuffer::writeString(string);
  }

  inline void println(const char* string) {
    Framebuffer::writeString(string);
    Framebuffer::writeCharacter('\n');
  }
}
