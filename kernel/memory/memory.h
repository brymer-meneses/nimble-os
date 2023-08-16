#pragma once

#include <lib/types.h>

namespace Memory {
  constexpr u64 PAGE_SIZE = 0x1000;

  auto initialize() -> void;
}

namespace Kernel {
  auto malloc(size_t size) -> void*;
  auto free(void* address) -> void;
}
