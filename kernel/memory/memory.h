#pragma once

#include <lib/types.h>
#include <kernel/memory/heap_allocator.h>

namespace memory {
  auto initialize() -> void;
  auto getKernelPageMap() -> uintptr_t*;
  auto subHHDM(const u64 virtualAddress) -> u64;
  auto addHHDM(const u64 physicalAddress) -> u64;
}

namespace kernel {
  auto malloc(size_t size) -> void*;
  auto free(void* address) -> void;
  auto getHeapAllocator() -> HeapAllocator&;
}
