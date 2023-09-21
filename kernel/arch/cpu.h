#pragma once
#include <lib/types.h>
#include <kernel/memory/vmm.h>

namespace arch::cpu {

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

  extern inline auto writeCR3(void* address) -> void {
    asm volatile ("mov %0, %%cr3" :: "r" ((u64) address) : "memory");
  }

  struct Context {
    u64 r15; // 0
    u64 r14; // 8
    u64 r13; // 16
    u64 r12; // 24
    u64 r11; // 32
    u64 r10; // 40
    u64 r9;  // 48
    u64 r8;  // 56
    u64 rdi; // 64
    u64 rsi; // 72
    u64 rdx; // 80
    u64 rcx; // 88
    u64 rbx; // 96
    u64 rax; // 104

    u64 vector_number; // 112
    u64 error_code;    // 120

    struct {
      u64 rip;
      u64 cs;
      u64 flags;
      u64 rsp;
      u64 ss;
    } iret;

    using Function = void(*)();

    Context(Function func, bool isUserAccessible);
    ~Context() = default;
  } __attribute__((packed));

#endif

}
