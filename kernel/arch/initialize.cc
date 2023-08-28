#include "platform.h"
#include "x86_64/interrupt/pic.h"
#include <kernel/utils/logger.h>

auto Arch::initialize() -> void {

#ifdef __x86_64__
  x86_64::PIC::initialize();
  x86_64::PIC::maskAll();
  x86_64::GDT::initialize();
  x86_64::IDT::initialize();
  x86_64::Interrupt::initialize();
  x86_64::Paging::initialize();

  Log::info("Successfully Initialized x86_64 components");
#endif

#ifdef __aarch64__
#endif

}

