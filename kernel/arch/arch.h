#pragma once

#ifdef __x86_64__
#include "x86_64/gdt/gdt.h"
#include "x86_64/interrupt/idt.h"
#include "x86_64/interrupt/interrupt.h"
#include "x86_64/paging/paging.h"
#endif

namespace arch {
  auto initialize() -> void;

#ifdef __x86_64__
  namespace interrupt = x86_64::interrupt;
  namespace paging = x86_64::paging;
  constexpr u64 PAGE_SIZE = 0x1000;
#endif

}

