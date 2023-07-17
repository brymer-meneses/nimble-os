#pragma once
#include <stdint.h>
#include <lib/types.h>

namespace Interrupt {

// Note: As much as i want this to inherit from `Format::FormatArgument`
// so we can trivially print it using `Kernel::println`, it kinda messes up the
// placement if we did so.
struct InterruptFrame {
  u64 r15;
  u64 r14;
  u64 r13;
  u64 r12;
  u64 r11;
  u64 r10;
  u64 r9;
  u64 r8;
  u64 rdi;
  u64 rsi;
  u64 rdx;
  u64 rcx;
  u64 rbx;
  u64 rax;

  u64 vector_number;
  u64 error_code;

  u64 iret_rip;
  u64 iret_cs;
  u64 iret_flags;
  u64 iret_rsp;
  u64 iret_ss;
};

  using ExceptionHandler = void (*)(InterruptFrame* context);
  using IrqHandler = void (*)();

  void setExceptionHandler(u16 interruptNumber, ExceptionHandler handler);
  void setIrqHandler(u16 interruptNumber, IrqHandler handler);

  void initialize();

}
