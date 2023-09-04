#pragma once

#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>
#include <kernel/arch/arch.h>

#include <source_location>

namespace kernel {

#ifdef DEBUG
  template <typename FormatArg, typename... FormatArgs>
  auto assert(bool condition, FormatArg arg, FormatArgs... args, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    serial::println("Assertion Failed at {}", loc.file_name());
    serial::println("  {} | {}", loc.line(), loc.function_name());
    serial::print("  -> ");
    serial::println(arg, args...);

    arch::debug::performStacktrace();

    kernel::halt();
  }

  inline auto assert(bool condition, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    serial::println("Assertion Failed at {}", loc.file_name());
    serial::println("  {} | {}", loc.line(), loc.function_name());
    arch::debug::performStacktrace();
    kernel::halt();
  }
#else
  // this should be optimized away
  template <typename... FormatArgs>
  inline auto assert(bool, FormatArgs...) -> void { }
  inline auto assert(bool) -> void {}
#endif


}

