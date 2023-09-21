#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <kernel/utils/logger.h>
#include <kernel/arch/arch.h>
#include <kernel/arch/cpu.h>
#include <kernel/boot/boot.h>
#include <kernel/memory/memory_map.h>
#include <lib/syslib/math.h>

#include <limine.h>

#include "memory.h"
#include "pmm.h"
#include "vmm.h"

#include "heap_allocator.h"

using arch::PAGE_SIZE;

static HeapAllocator kernelHeap;
static VMM kernelVMM;

static u64 hhdmOffset = 0;

// this should be defined at `linker.ld`
extern uintptr_t kernel_start_address;
extern uintptr_t kernel_end_address;

static uintptr_t* kernelPageMap = nullptr;

auto memory::addHHDM(u64 physicalAddress) -> u64 {
  kernel::assert(hhdmOffset, "VMM not initialized");

  return physicalAddress + hhdmOffset;
}

auto memory::subHHDM(u64 virtualAddress) -> u64 {
  kernel::assert(hhdmOffset, "VMM not initialized");

  return virtualAddress - hhdmOffset;
}

auto memory::getKernelPageMap() -> uintptr_t* {
  return kernelPageMap;
}

auto memory::initialize() -> void {

  PMM::initialize();

  if (!boot::hhdmRequest.response) {
    kernel::panic("HHDM Request is null");
  }
  hhdmOffset = boot::hhdmRequest.response->offset;

  kernelPageMap = arch::paging::getInitialPageMap();

  auto kernelHeapStart = sl::math::alignUp((u64) &kernel_end_address, PAGE_SIZE) + PAGE_SIZE;

  log::debug("initializing kernel vmm");
  kernelVMM.initialize(kernelPageMap, kernelHeapStart, {.userAccessible=false, .writeable=true, .executable=true});

  log::debug("initializing kernel heap");
  kernelHeap.initialize(&kernelVMM);

  log::info("Successfully initialized Memory.");
}

auto memory::createPageMap() -> uintptr_t* {
  auto pagemap = (uintptr_t*) PMM::allocatePage();
  for (size_t i = 256; i < 512; i++) {
    pagemap[i] = kernelPageMap[i];
  }
  return pagemap;
}

auto memory::allocateStack(uintptr_t* pagemap, bool isUserAccessible) -> uintptr_t {
  kernel::assert(pagemap != nullptr, "pagemap must not be a nullptr");

  constexpr auto stackTop = 0xFFFFFFFFFFFFF000;
  constexpr auto stackBottom = stackTop - 4 * PAGE_SIZE;

  VMFlag flags = {.userAccessible=isUserAccessible, .writeable=true, .executable=true};

  for (int i = 0; i < 4; i++) {
    auto page = (uintptr_t) PMM::allocatePage();
    arch::paging::map(pagemap, stackBottom + i * PAGE_SIZE, page, flags);
  }

  return stackTop;
}

auto operator new (size_t size) -> void* {
  return kernelHeap.alloc(size);
}

auto operator delete (void* ptr) noexcept -> void {
  kernelHeap.free(ptr);
}

auto kernel::malloc(size_t size) -> void* {
  return kernelHeap.alloc(size);
}

auto kernel::free(void* address) -> void {
  return kernelHeap.free(address);
}

auto kernel::getHeapAllocator() -> HeapAllocator* {
  return &kernelHeap;
}

auto kernel::getKernelVMM() -> VMM& {
  return kernelVMM;
}

