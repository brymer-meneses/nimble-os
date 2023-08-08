#include "platform.h"

auto Arch::initialize() -> void {

#ifdef __x86_64__
  x86_64::GDT::initialize();
  x86_64::IDT::initialize();
  x86_64::Interrupt::initialize();
  x86_64::Paging::initialize();
#endif

#ifdef __aarch64__
#endif

}

