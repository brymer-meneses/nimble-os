#include "stacktrace.h"
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>

using namespace x86_64;

struct StackFrame {
  StackFrame* rbp;
  u64 rip;
};

auto debug::performStacktrace() -> void {

  struct StackFrame* stack;
  asm volatile ("mov %%rbp, %0" : "=r" (stack));
  serial::println("\nStack trace: ");

  while (stack->rip != 0) {
    serial::println("\t{#0x16}", stack->rip);
    stack = stack->rbp;
  }

}
