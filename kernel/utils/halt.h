#pragma once

namespace Kernel {
  extern inline auto halt() -> void {
    for (;;) {
      asm volatile ("hlt");
    }
  }
}
