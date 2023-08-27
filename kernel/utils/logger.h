#pragma once
#include <type_traits>
#include <lib/types.h>
#include <kernel/drivers/serial.h>

#ifdef LOG_ALL
  #define LOG_HEAP
  #define LOG_VMM
  #define LOG_PMM
  #define LOG_GDT
  #define LOG_ARCH
#endif

#define DECLARE_LOGGING_MDOULE(name) \
  struct __##name { \
    static constexpr const char* stringRep = #name; \
  }; \
  constexpr auto name = __##name {};

#define PRINT_LOG_LEVEL() \
  if (level >= LogLevel::Debug) { \
    serial::print("[DEBUG]: "); \
  } else if (level >= LogLevel::Info) {\
    serial::print("[INFO]: "); \
  } else if (level >= LogLevel::Warning) {\
    serial::print("[WARN]: "); \
  }

#define DEFINE_MODULE(name) \
  if constexpr (std::is_same_v<decltype(module), Mod::__##name>) { \
    PRINT_LOG_LEVEL(); \
    serial::print("{} ", Mod::__##name::stringRep); \
    serial::println(string, args...); \
  }

namespace Log {

  namespace Mod {
    DECLARE_LOGGING_MDOULE(VMM);
    DECLARE_LOGGING_MDOULE(PMM);
    DECLARE_LOGGING_MDOULE(Heap);
  };

  enum LogLevel : u8 {
    Debug = 0,
    Info = 1,
    Warning = 2,
  };


  template <LogLevel level, typename ...Args>
  auto internalLog(auto module, const char* string, Args... args) -> void {

  #ifdef LOG_HEAP
    DEFINE_MODULE(Heap);
  #endif

  #ifdef LOG_PMM
    DEFINE_MODULE(PMM);
  #endif

  #ifdef LOG_VMM
    DEFINE_MODULE(VMM);
  #endif

  }

  template <typename ...Args>
  auto debug(auto module, const char* string, Args... args) -> void {
    internalLog<LogLevel::Debug>(module, string, args...);
  }

  template <typename ...Args>
  auto warning(auto module, const char* string, Args... args) -> void {
    internalLog<LogLevel::Warning>(module, string, args...);
  }

  template <typename ...Args>
  auto info(auto module, const char* string, Args... args) -> void {
    internalLog<LogLevel::Info>(module, string, args...);
  }
}

namespace Mod = Log::Mod;

