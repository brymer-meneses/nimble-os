#pragma once
#include <type_traits>
#include <lib/types.h>
#include <kernel/drivers/serial.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

namespace Log {

  enum class LogLevel : u8 {
    Debug = 0,
    Info = 1,
    Warning = 2,
  };

  namespace internal {
    template <LogLevel level, typename ...Args>
    auto logImpl(const char* string, Args... args) -> void {
      auto writer = serial::getWriter();
      switch (level) {
        case LogLevel::Debug:
          writer.writeString("[ DEBUG ]: ");
          break;
        case LogLevel::Warning:
          writer.writeString("[ WARN ]: ");
          break;
        case LogLevel::Info:
          writer.writeString("[ INFO ]: ");
          break;
      };
      sl::format(writer, string, args...);
      writer.writeChar('\n');
    }

  }

  template <typename ...Args>
  auto debug(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Debug) {
      internal::logImpl<LogLevel::Debug>(string, args...);
    }
  }

  template <typename ...Args>
  auto warning(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Warning) {
      internal::logImpl<LogLevel::Warning>(string, args...);
    }
  }

  template <typename ...Args>
  auto info(const char* string, Args... args) -> void {
    if constexpr (LOG_LEVEL <= (u8) LogLevel::Info) {
      internal::logImpl<LogLevel::Info>(string, args...);
    }
  }
}


