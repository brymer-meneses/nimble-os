#pragma once
#include <lib/types.h>

namespace IDT {

  constexpr u16 MAX_ENTRIES = 256;
  void initialize();
  auto allocateVector() -> u8;
}
