#pragma once

#include <lib/types.h>

namespace PIC {
  
  void initialize();
  void maskAll();
  void sendEndOfInterrupt(u8 irq);
  void clearMask(u8 irq);
}
