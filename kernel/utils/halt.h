#pragma once

namespace Kernel {
  [[noreturn]] extern inline auto halt() -> void {
    for (;;) {
      asm volatile ("hlt");
    }
  }
}
