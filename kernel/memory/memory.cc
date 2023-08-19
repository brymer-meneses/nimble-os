#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/arch/platform.h>
#include <lib/syslib/math.h>

#include <limine.h>

#include "memory.h"
#include "pmm.h"
#include "vmm.h"

#include "heap_allocator.h"

using Arch::PAGE_SIZE;

static HeapAllocator kernelHeapAllocator;

// this should be defined at `linker.ld`
extern "C" uintptr_t kernel_end;

auto Memory::initialize() -> void {
  PMM::initialize();
  VMM::initialize();

  auto flags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  const auto kernelHeapAddress = sl::math::alignUp((u64) &kernel_end, PAGE_SIZE) + PAGE_SIZE;

  kernelHeapAllocator.initialize(kernelHeapAddress, 30 * PAGE_SIZE, flags);
}

auto Kernel::malloc(size_t size) -> void* {
  return kernelHeapAllocator.alloc(size);
}

auto Kernel::free(void* address) -> void {
  return kernelHeapAllocator.free(address);
}

auto Kernel::getHeapAllocator() -> HeapAllocator& {
  return kernelHeapAllocator;
}
