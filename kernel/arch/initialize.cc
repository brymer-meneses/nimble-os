#include "prelude.h"

auto Arch::initialize() -> void {

#ifdef __x86_64__
  x86_64::GDT::initialize();
  x86_64::IDT::initialize();
  x86_64::Interrupt::initialize();
#endif

}

