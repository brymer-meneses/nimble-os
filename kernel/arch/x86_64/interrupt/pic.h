#pragma once

#include <lib/types.h>

namespace x86_64::PIC {
  
  auto initialize() -> void;
  auto maskAll() -> void;
  auto sendEOI(u8 irq) -> void;
  auto clearMask(u8 irq) -> void;
}
