#pragma once
#include <stdint.h>

namespace Interrupt {

// Note: As much as i want this to inherit from `Format::FormatArgument`
// so we can trivially print it using `Kernel::println`, it kinda messes up the
// placement if we did so.
struct InterruptFrame {
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;

  uint64_t vector_number;
  uint64_t error_code;

  uint64_t iret_rip;
  uint64_t iret_cs;
  uint64_t iret_flags;
  uint64_t iret_rsp;
  uint64_t iret_ss;
};

  using InterruptHandler = void (*)(InterruptFrame* context);
  void setHandler(uint16_t interruptNumber, InterruptHandler handler);
  void initialize();

}
