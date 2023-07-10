#include <stdint.h>
#include <lib/kernel/print.h>
#include <lib/kernel/panic.h>

struct CpuStatus {
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
} __attribute__((packed));


// this is called from `idt.asm` 
extern "C" void interruptDispatch(CpuStatus* context) {
  Kernel::println("Exception {}", context->vector_number);

  switch (context->vector_number) {
    case 0:
      Kernel::println("Division by zero.");
      break;
    case 13:
      Kernel::println("General protection fault.");
      break;
    case 14:
      Kernel::println("General protection fault.");
      break;
    default:
      Kernel::println("Unexpected Interrupt");
      break;
  }
  return;
}
