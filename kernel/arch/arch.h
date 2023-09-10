#pragma once

#include <kernel/arch/cpu.h>

#ifdef __x86_64__
#include "x86_64/interrupt/interrupt.h"
#include "x86_64/paging/paging.h"
#include "x86_64/debug/stacktrace.h"
#endif

namespace arch {
  auto initialize() -> void;

#ifdef __x86_64__
  constexpr u64 PAGE_SIZE = 0x1000;

  namespace interrupt = x86_64::interrupt;
  namespace paging = x86_64::paging;
  namespace debug = x86_64::debug;
#endif

  extern "C" auto switchContext(arch::cpu::Context* context) -> void;
}

