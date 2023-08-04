#pragma once

#include <lib/types.h>
#include "address.h"
#include "vmm.h"

namespace Memory {
  constexpr u64 PAGE_SIZE = 0x1000;

  auto initialize() -> void;
}
