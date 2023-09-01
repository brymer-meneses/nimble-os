#pragma once

namespace kernel {
  [[noreturn]] extern inline auto halt() -> void {
    for (;;) {
      asm volatile ("hlt");
    }
  }
}
