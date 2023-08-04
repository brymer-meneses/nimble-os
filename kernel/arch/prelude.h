
#ifdef __x86_64__
#include "x86_64/gdt/gdt.h"
#include "x86_64/interrupt/idt.h"
#include "x86_64/interrupt/interrupt.h"
#endif

// TODO: create APIs for allocating interrupt vector and other stuff
namespace Arch {
  auto initialize() -> void;
}

