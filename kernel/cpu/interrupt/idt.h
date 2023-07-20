#pragma once
#include <lib/types.h>

namespace IDT {

  constexpr u16 MAX_ENTRIES = 256;
  auto initialize() -> void;
  auto allocateVector() -> u8;
}
