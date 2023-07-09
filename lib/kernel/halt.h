#pragma once

namespace Kernel {
  extern inline void halt() {
    asm volatile ("cli");
    for (;;) {
      asm volatile ("hlt");
    }
  }
}
