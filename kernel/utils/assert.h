#pragma once

#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/graphics/framebuffer.h>

#include <source_location>

namespace kernel {

#ifdef DEBUG
  template <typename FormatArg, typename... FormatArgs>
  auto assert(bool condition, FormatArg arg, FormatArgs... args, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    kernel::print("[ Assertion Failed ]: {}:{} {} ", loc.file_name(), loc.function_name(), loc.line());
    kernel::println(arg, args...);
    kernel::halt();
  }

  inline auto assert(bool condition, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    kernel::println("[ Assertion Failed ]: {} {} {}", loc.function_name(), loc.file_name(), loc.line());
    kernel::halt();
  }
#else
  // this should be optimized away
  template <typename... FormatArgs>
  inline auto assert(bool, FormatArgs...) -> void { }
  inline auto assert(bool) -> void {}
#endif


}

