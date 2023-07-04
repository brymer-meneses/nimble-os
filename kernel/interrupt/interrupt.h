#pragma once

#include <stdint.h>
#include <optional>

namespace interrupt {

struct cpu_status {
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

void initialize_idt();
void set_descriptor(uint8_t vector, uint64_t handler, uint8_t dpl);

std::optional<cpu_status> get_context_on_last_interrupt();

}
