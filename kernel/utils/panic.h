#pragma once

#include <kernel/graphics/framebuffer.h>
#include <kernel/utils/panic.h>
#include <lib/syslib/format.h>
#include <lib/types.h>
#include "halt.h"

namespace Kernel {

  struct PanicWriter : sl::FormatWriter {
    auto writeChar(const char character) -> void final {
      Framebuffer::setForeground(0xBF616A);
      Framebuffer::writeCharacter(character);
    }
  };

  namespace {
    PanicWriter panicWriter{};
  }

  [[noreturn]] inline auto panic(const char* string) -> void {
    panicWriter.writeString(string);
    Kernel::halt();
  }

  template<typename ...Args>
  [[noreturn]] auto panic(const char* string, Args... args) -> void {
    sl::format(panicWriter, string, args...);
    Kernel::halt();
  }

}
