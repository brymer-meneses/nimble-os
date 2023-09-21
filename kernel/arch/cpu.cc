#include "cpu.h"
#include <kernel/memory/memory.h>
#include <kernel/utils/logger.h>

using namespace arch::cpu;

static constexpr u8 KERNEL_CS = 0x8;
static constexpr u8 KERNEL_SS = 0x10;

Context::Context(Function func, bool isUserContext) {
  rdi = 0;

  iret.cs = KERNEL_CS;
  iret.ss = KERNEL_SS;
  iret.rip = (u64) func;
  iret.flags = 0x202;
  iret.rsp = memory::allocateStack(memory::getKernelPageMap(), isUserContext);
}
