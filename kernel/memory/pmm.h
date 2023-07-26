#pragma once

#include <lib/types.h>

using VirtualAddress = u64;
using PhysicalAddress = u64;

namespace PMM {
  constexpr u64 PAGE_SIZE = 4096;
  auto initialize() -> void;
}
