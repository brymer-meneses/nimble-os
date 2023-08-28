#pragma once
#include <lib/syslib/format.h>

namespace serial {

  struct SerialWriter : public sl::FormatWriter {
    auto writeChar(const char character) -> void final;
    SerialWriter() {};
  };

  auto initialize() -> void;
  auto getWriter() -> SerialWriter&;

  template<typename ...Args>
  auto println(const char* string, Args... args) -> void {
    auto& writer = getWriter();
    sl::format(writer, string, args...);
    writer.writeChar('\n');
  }

  template<typename ...Args>
  auto print(const char* string, Args... args) -> void {
    auto& writer = getWriter();
    sl::format(writer, string, args...);
  }
}
