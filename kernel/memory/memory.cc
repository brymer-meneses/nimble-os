#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

#include <limine.h>

#include "memory.h"
#include "memory_map.h"
#include "pmm.h"
#include "vmm.h"

#include "heap_allocator.h"

static HeapAllocator kernelHeapAllocator;

auto Memory::initialize() -> void {
  PMM::initialize();
  VMM::initialize();

  auto flags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  kernelHeapAllocator.initialize(VMM::addHHDM(0), 5 * PAGE_SIZE, flags);
}

auto Kernel::malloc(size_t size) -> void* {
  return kernelHeapAllocator.alloc(size);
}

auto Kernel::free(void* address) -> void {
  return kernelHeapAllocator.free(address);
}
