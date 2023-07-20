#pragma once

#include <lib/types.h>

namespace PIC {
  
  auto initialize() -> void;
  auto maskAll() -> void;
  auto sendEndOfInterrupt(u8 irq) -> void;
  auto clearMask(u8 irq) -> void;
}
