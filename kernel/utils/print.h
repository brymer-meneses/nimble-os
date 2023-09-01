#pragma once
#include <kernel/graphics/framebuffer.h>
#include <lib/syslib/format.h>

namespace {
  struct PrintWriter : sl::FormatWriter {
    auto writeChar(const char character) -> void final {
      framebuffer::writeCharacter(character);
    }
  };

}

namespace kernel {

  template<typename ...Args>
  auto print(const char* string, Args ...args) -> void {
    PrintWriter writer{};
    if constexpr (sizeof...(args) == 0) {
      writer.writeString(string);
    } else {
      sl::format(writer, string, args...);
    }
  }

  template<typename ...Args>
  auto println(const char* string, Args ...args) -> void {
    PrintWriter writer{};
    if constexpr (sizeof...(args) == 0) {
      writer.writeString(string);
      writer.writeChar('\n');
    } else {
      sl::format(writer, string, args...);
      writer.writeChar('\n');
    }

  }

}
