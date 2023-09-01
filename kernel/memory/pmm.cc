#include "pmm.h"

#include <kernel/utils/print.h>
#include <kernel/utils/logger.h>
#include <kernel/utils/panic.h>

#include <kernel/utils/assert.h>
#include <lib/thirdparty/limine.h>
#include <lib/types.h>

#include <cstddef>

#include "memory_map.h"
#include "bitmap_allocator.h"

static BitmapAllocator allocator;

auto PMM::initialize() -> void {
  allocator.initialize();
  log::info("Initialized PMM");
}

auto PMM::allocatePage() -> void* {
  auto page = allocator.allocatePage();
  if (!page) {
    kernel::panic("PMM: cannot allocate page");
  }
  return reinterpret_cast<void*>(page.value());
}

auto PMM::freePage(uintptr_t address) -> void {
  allocator.freePage(address);
}


