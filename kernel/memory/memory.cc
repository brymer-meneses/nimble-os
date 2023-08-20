#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <kernel/arch/platform.h>
#include <lib/syslib/math.h>

#include <limine.h>

#include "memory.h"
#include "pmm.h"
#include "vmm.h"

#include "heap_allocator.h"

using Arch::PAGE_SIZE;

// this should be defined at `linker.ld`
extern uintptr_t kernel_address_end;

static HeapAllocator kernelHeap;
static VMM kernelVMM;

static volatile auto hhdmRequest = limine_hhdm_request {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0,
};

static u64 hhdmOffset = 0;

auto Memory::addHHDM(u64 physicalAddress) -> u64 {
  Kernel::assert(hhdmOffset, "VMM not initialized");

  return physicalAddress + hhdmOffset;
}

auto Memory::subHHDM(u64 virtualAddress) -> u64 {
  Kernel::assert(hhdmOffset, "VMM not initialized");

  return virtualAddress - hhdmOffset;
}

auto Memory::initialize() -> void {
  PMM::initialize();

  if (!hhdmRequest.response) {
    Kernel::panic("HHDM Request is null");
  }
  hhdmOffset = hhdmRequest.response->offset;

  auto flags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  auto kernelHeapStart = sl::math::alignUp((u64) &kernel_address_end, PAGE_SIZE) + 2 * PAGE_SIZE;

  Kernel::println("Heap start {hex}", kernelHeapStart);

  kernelVMM.initialize(kernelHeapStart, flags);
  kernelHeap.initialize(&kernelVMM);
}


auto Kernel::malloc(size_t size) -> void* {
  return kernelHeap.alloc(size);
}

auto Kernel::free(void* address) -> void {
  return kernelHeap.free(address);
}

auto Kernel::getHeapAllocator() -> HeapAllocator& {
  return kernelHeap;
}
