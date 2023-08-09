#pragma once
#include <kernel/utils/format.h>
#include <kernel/graphics/framebuffer.h>

namespace Kernel {
  template<typename Arg, typename ...Args>
  auto print(const char* string, Arg arg, Args ...args) -> void {
    char buffer[256];
    Format::format(buffer, string, arg, args...);
    Framebuffer::writeString(buffer);
  }

  template<typename Arg, typename ...Args>
  auto println(const char* string, Arg arg, Args ...args) -> void {
    char buffer[256];
    Format::format(buffer, string, arg, args...);
    Framebuffer::writeString(buffer);
    Framebuffer::writeCharacter('\n');
  }

  inline auto print(const char* string) -> void {
    Framebuffer::writeString(string);
  }

  inline auto println(const char* string) -> void {
    Framebuffer::writeString(string);
    Framebuffer::writeCharacter('\n');
  }
}
