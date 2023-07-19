#pragma once
#include <lib/types.h>

namespace LAPIC {
  void initialize();

  void write(u32 reg, u32 value);

  auto read(u32 reg) -> u32;
}
