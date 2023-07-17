#pragma once

namespace Kernel {
  extern inline void halt() {
    for (;;) {
      asm volatile ("hlt");
    }
  }
}
