#pragma once
#include <stdint.h>

namespace gdt {
  

  void initialize();
  void set_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
  
}
