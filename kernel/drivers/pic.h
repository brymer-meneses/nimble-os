#pragma once

#include <stdint.h>

namespace pic {

  void remap_interrupts(int offset, int offset2);
  void finish_interrupt(uint8_t irq);
  void clear_mask(uint8_t irqline);
  void set_mask(uint8_t irqline);
  void disable();
  void initialize();
}

