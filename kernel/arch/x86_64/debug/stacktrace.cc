#include "stacktrace.h"
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>

#ifdef DEBUG
#include "build/symbols.h"
#endif

using namespace x86_64;

struct StackFrame {
  StackFrame* rbp;
  u64 rip;
};

static auto getSymbolName(u64 address) -> const char* {

#ifdef DEBUG
  for (auto& symbol : symbols) {
    if (symbol.address <= address and address <= symbol.address + symbol.size) {
      return symbol.name;
    }
  }
#endif

  return "unknown";
}

auto debug::performStacktrace() -> void {

  struct StackFrame* stack;
  asm volatile ("mov %%rbp, %0" : "=r" (stack) ::);
  serial::println("\nStack trace: ");

  while (stack->rip != 0) {
    serial::println("\t<{}>", getSymbolName(stack->rip));
    stack = stack->rbp;
  }

}
