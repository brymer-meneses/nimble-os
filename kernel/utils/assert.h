#pragma once

#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <source_location>


#define KERNEL_ASSERT(condition) \
  if (!(condition)) { \
    const auto location = std::source_location::current(); \
    Kernel::println("Assertion Failed at {} {} {} {}", \
      location.function_name(),  \
      location.file_name(),  \
      location.line(),  \
      location.column()); \
    Kernel::halt(); \
  }
