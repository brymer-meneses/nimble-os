#include "arch.h"
#include <kernel/memory/memory.h>

#ifdef __x86_64__
#include "x86_64/interrupt/pic.h"
#include "x86_64/gdt/gdt.h"
#endif

#include <kernel/utils/logger.h>
#include <kernel/utils/assert.h>


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


