#pragma once
#include <type_traits>
#include <lib/types.h>
#include <kernel/drivers/serial.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

namespace log {

  enum class LogLevel : u8 {
    Debug = 0,
    Info = 1,
    Warning = 2,
  };


  template <typename ...Args>
  auto debug(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Debug) {
      auto writer = serial::getWriter();
      writer.writeString("[ DEBUG ]: ");
      sl::format(writer, string, args...);
      writer.writeChar('\n');

    }
  }

  template <typename ...Args>
  auto warn(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Warning) {
      auto writer = serial::getWriter();

      writer.writeString("[ WARN ]: ");
      sl::format(writer, string, args...);
      writer.writeChar('\n');

    }
  }

  template <typename ...Args>
  auto info(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Info) {
      auto writer = serial::getWriter();

      writer.writeString("[ INFO ]: ");
      sl::format(writer, string, args...);
      writer.writeChar('\n');

    }
  }
}


