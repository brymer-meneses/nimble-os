#pragma once

#include <lib/types.h>
#include <kernel/memory/heap_allocator.h>

namespace Memory {

  auto initialize() -> void;
}

namespace Kernel {
  auto malloc(size_t size) -> void*;
  auto free(void* address) -> void;
  auto getHeapAllocator() -> HeapAllocator&;
}
