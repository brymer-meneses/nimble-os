#pragma once
#include <lib/types.h>

namespace CPU {

#ifdef __x86_64__
  extern inline auto readCR2() -> u64 {
    u64 value;
    asm volatile ("movq %%cr2, %0" : "=r"(value));
    return value;
  };

  extern inline auto readCR3() -> u64 {
    u64 value;
    asm volatile ("movq %%cr3, %0" : "=r"(value));
    return value;
  };

  extern inline auto write(void* address) -> void {
    asm volatile ("mov %0, %%cr3" :: "r" ((u64) address) : "memory");
  }
#endif

}
