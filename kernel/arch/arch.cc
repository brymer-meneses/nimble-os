#include "arch.h"
#include <kernel/memory/memory.h>

#ifdef __x86_64__
#include "x86_64/interrupt/pic.h"
#include "x86_64/gdt/gdt.h"
#endif

#include <kernel/utils/logger.h>
#include <kernel/utils/assert.h>

static constexpr u8 KERNEL_CS = 0x8;
static constexpr u8 KERNEL_SS = 0x10;

auto arch::initialize() -> void {

#ifdef __x86_64__
  using namespace x86_64;

  PIC::initialize();
  PIC::maskAll();

  GDT::initialize();

  interrupt::initialize();

  paging::initialize();

  log::info("Successfully Initialized x86_64 components");
#endif

#ifdef __aarch64__
#endif

}

auto arch::initializeContext(cpu::Context* ctx, Function func, VMFlag flags) -> void {
  ctx->iret.cs = KERNEL_CS;
  ctx->iret.ss = KERNEL_SS;
  ctx->iret.rip = (u64) func;
  ctx->iret.flags = 0x202;
  ctx->iret.rsp = memory::allocateStack(memory::getKernelPageMap(), flags);
}

