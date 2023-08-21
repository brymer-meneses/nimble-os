#pragma once
#include <type_traits>

enum class Mod {
   VMM,
   PMM,
   Heap,
   GDT,
};

namespace {
  #include <lib/types.h>
  #include <kernel/drivers/serial.h>

  enum LogLevel : u8 {
    Debug = 0,
    Info = 1,
    Warning = 2,
  };

  template <Mod module, LogLevel level, typename ...Args>
  auto internalLog(const char* string, Args... args) -> void {

    if (level >= LogLevel::Debug) {
      serial::print("[  DEBUG  ]: ");
    } else if (level >= LogLevel::Info) {
      serial::print("[  INFO  ]: ");
    } else if (level >= LogLevel::Warning) {
      serial::print("[  WARN  ]: ");
    };

  #ifdef LOG_HEAP
    if constexpr (module == Mod::Heap) {
      serial::println(string, args...);
      return;
    }
  #endif

  #ifdef LOG_VMM
    if constexpr (module == Mod::VMM) {
      serial::println(string, args...);
    }
  #endif

  #ifdef LOG_PMM
    if constexpr (module == Mod::PMM) {
      serial::println(string, args...);
    }
  #endif

  #ifdef LOG_GDT
    if constexpr (module == Mod::GDT) {
      serial::println(string, args...);
    }
  #endif
  }
}

namespace log {
  template <Mod module, typename ...Args>
  auto debug(const char* string, Args... args) -> void {
    internalLog<module, LogLevel::Debug>(string, args...);
  }

  template <Mod module, typename ...Args>
  auto warning(const char* string, Args... args) -> void {
    internalLog<module, LogLevel::Warning>(string, args...);
  }

  template <Mod module, typename ...Args>
  auto info(const char* string, Args... args) -> void {
    internalLog<module, LogLevel::Info>(string, args...);
  }

  // log::info<Mod::PMM>("hi");
}
