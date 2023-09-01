#pragma once

#include <kernel/graphics/framebuffer.h>
#include <kernel/utils/panic.h>
#include <lib/syslib/format.h>
#include <lib/types.h>
#include "halt.h"

namespace kernel {

  struct PanicWriter : sl::FormatWriter {
    auto writeChar(const char character) -> void final {
      framebuffer::setForeground(0xBF616A);
      framebuffer::writeCharacter(character);
    }
  };

  [[noreturn]] inline auto panic(const char* string) -> void {
    PanicWriter panicWriter{};
    panicWriter.writeString(string);
    kernel::halt();
  }

  template<typename ...Args>
  [[noreturn]] auto panic(const char* string, Args... args) -> void {
    PanicWriter panicWriter{};
    sl::format(panicWriter, string, args...);
    kernel::halt();
  }

}
