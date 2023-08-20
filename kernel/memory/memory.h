#pragma once

#include <lib/types.h>
#include <kernel/memory/heap_allocator.h>

namespace Memory {
  auto initialize() -> void;
  auto subHHDM(const u64 virtualAddress) -> u64;
  auto addHHDM(const u64 physicalAddress) -> u64;
}

namespace Kernel {
  auto malloc(size_t size) -> void*;
  auto free(void* address) -> void;
  auto getHeapAllocator() -> HeapAllocator&;
}
