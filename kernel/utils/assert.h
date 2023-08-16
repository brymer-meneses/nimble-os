#pragma once

#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/graphics/framebuffer.h>

#include <source_location>

namespace Kernel {

#ifdef DEBUG
  template <typename FormatArg, typename... FormatArgs>
  auto assert(bool condition, FormatArg arg, FormatArgs... args, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    Kernel::print("[ Assertion Failed ]: {}:{} {}:{} ", 
                  loc.file_name(), loc.function_name(), loc.line(), loc.column());
    Kernel::println(arg, args...);
    Kernel::halt();
  }

  inline auto assert(bool condition, const std::source_location loc = std::source_location::current()) -> void {
    if (condition) [[likely]] return;

    Kernel::println("[ Assertion Failed ]: {} {} {} {}", loc.function_name(),
                    loc.file_name(), loc.line(), loc.column());
    Kernel::halt();
  }
#else
  // this should be optimized away
  template <typename... FormatArgs>
  inline auto assert(bool, FormatArgs...) -> void { }
  inline auto assert(bool) -> void {}
#endif


}

