#include "pmm.h"

#include <kernel/utils/print.h>
#include <kernel/utils/panic.h>

#include <kernel/utils/assert.h>
#include <lib/thirdparty/limine.h>
#include <lib/types.h>
#include <lib/math.h>

#include <cstddef>

#include "memory_map.h"
#include "bitmap_allocator.h"

static BitmapAllocator allocator;

auto PMM::initialize() -> void {
  allocator.initialize();
}

auto PMM::allocatePage() -> void* {
  auto page = allocator.allocatePage();
  if (!page) {
    Kernel::panic("PMM: cannot allocate page");
  }
  return reinterpret_cast<void*>(page.value());
}

auto PMM::allocateContiguousPages(size_t num) -> void* {
  auto pages = allocator.allocateContiguousPages(num);
  if (!pages) {
    Kernel::panic("PMM: cannot allocate page");
  }
  return reinterpret_cast<void*>(pages.value());
}

auto PMM::freePage(uintptr_t address) -> void {
  allocator.freePage(address);
}

auto PMM::freeContiguousPages(uintptr_t address, size_t num) -> void {
  allocator.freeContiguousPages(address, num);
}

