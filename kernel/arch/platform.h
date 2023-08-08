#pragma once

#ifdef __x86_64__
#include "x86_64/gdt/gdt.h"
#include "x86_64/interrupt/idt.h"
#include "x86_64/interrupt/interrupt.h"
#include "x86_64/paging/paging.h"
#endif

namespace Arch {
  auto initialize() -> void;

#ifdef __x86_64__
  namespace Interrupt = x86_64::Interrupt;
  namespace Paging = x86_64::Paging;
#endif

}

