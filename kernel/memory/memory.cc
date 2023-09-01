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

// this should be defined at `linker.ld`

static HeapAllocator kernelHeap;
static VMM kernelVMM;


static u64 hhdmOffset = 0;

auto memory::addHHDM(u64 physicalAddress) -> u64 {
  kernel::assert(hhdmOffset, "VMM not initialized");

  return physicalAddress + hhdmOffset;
}

auto memory::subHHDM(u64 virtualAddress) -> u64 {
  kernel::assert(hhdmOffset, "VMM not initialized");

  return virtualAddress - hhdmOffset;
}

extern uintptr_t text_start_address;
extern uintptr_t text_end_address;

extern uintptr_t rodata_start_address;
extern uintptr_t rodata_end_address;

extern uintptr_t data_start_address;
extern uintptr_t data_end_address;

extern uintptr_t kernel_end_address;

static uintptr_t* kernelPageMap = nullptr;

static auto initializeKernelPageMap() -> void {

  auto defaultFlags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  auto readOnlyFlags = VMFlag {
    .userAccessible = false,
    .writeable = false,
    .executable = false,
  };

  auto textFlags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  auto dataFlags = VMFlag {
    .userAccessible = false,
    .writeable = true,
    .executable = true,
  };

  auto textStart = sl::math::alignDown((uintptr_t) &text_start_address, PAGE_SIZE);
  auto textEnd = sl::math::alignUp((uintptr_t) &text_end_address, PAGE_SIZE);

  auto rodataStart = sl::math::alignDown((uintptr_t) &rodata_start_address, PAGE_SIZE);
  auto rodataEnd = sl::math::alignUp((uintptr_t) &rodata_end_address, PAGE_SIZE);

  auto dataStart = sl::math::alignDown((uintptr_t) &data_start_address, PAGE_SIZE);
  auto dataEnd = sl::math::alignUp((uintptr_t) &data_end_address, PAGE_SIZE);

  auto kernelAddress = boot::kernelAddressRequest.response;

  kernelPageMap = (uintptr_t*) memory::addHHDM((uintptr_t) PMM::allocatePage());
  log::debug("Kernel Page Map at {#0x16}", kernelPageMap);


  for (size_t i = 256; i < 512; i++) {
    arch::paging::getNextLevel(kernelPageMap, i, defaultFlags, true);
  }

  log::debug("Mapping kernel text {#0x16} - {#0x16}", textStart, textEnd);
  for (uintptr_t textAddr = textStart; textAddr < textEnd; textAddr += PAGE_SIZE) {
    auto phys = textAddr - kernelAddress->virtual_base + kernelAddress->physical_base;
    arch::paging::map(kernelPageMap, textAddr, phys, textFlags);
  }

  log::debug("Mapping kernel read-only data {#0x16} - {#0x16}", rodataStart, rodataEnd);
  for (uintptr_t rodataAddr = rodataStart; rodataAddr < rodataEnd; rodataAddr += PAGE_SIZE) {
    auto phys = rodataAddr - kernelAddress->virtual_base + kernelAddress->physical_base;
    arch::paging::map(kernelPageMap, rodataAddr, phys, readOnlyFlags);
  }

  log::debug("Mapping kernel data {#0x16} - {#0x16}", dataStart, dataEnd);
  for (uintptr_t dataAddr = dataStart; dataAddr < dataEnd; dataAddr += PAGE_SIZE) {
    auto phys = dataAddr - kernelAddress->virtual_base + kernelAddress->physical_base;
    arch::paging::map(kernelPageMap, dataAddr, phys, dataFlags);
  }

  log::debug("Mapping address 0x1000 - 0x100000000");
  for (uintptr_t addr = 0x1000; addr < 0x100000000; addr += PAGE_SIZE) {
    arch::paging::map(kernelPageMap, addr, addr, {.writeable=true});
    arch::paging::map(kernelPageMap, memory::addHHDM(addr), addr, {.writeable=true});
  }

  auto* memmap = boot::memmapRequest.response;

  log::debug("Mapping memmap");
  for (size_t i = 0; i < memmap->entry_count; i++) {
    auto* entry = memmap->entries[i];
  
    uintptr_t base = sl::math::alignDown(entry->base, PAGE_SIZE);
    uintptr_t top = sl::math::alignUp(entry->base + entry->length, PAGE_SIZE);
  
    if (top <= 0x100000000) {
      continue;
    }
  
    for (uintptr_t j = base; j < top; j += PAGE_SIZE) {
      if (j < 0x100000000) {
        continue;
      }
  
      arch::paging::map(kernelPageMap, j, j, {.userAccessible=true, .writeable=true, .executable=true});
      arch::paging::map(kernelPageMap, memory::addHHDM(j), j, {.userAccessible=false, .writeable=true, .executable=true});
    }
  }

  log::debug("Switching pagemap");
  arch::paging::switchPageMap(kernelPageMap);
  log::debug("success");
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

  initializeKernelPageMap();

  auto kernelHeapStart = sl::math::alignUp((u64) &kernel_end_address, PAGE_SIZE) + PAGE_SIZE;

  log::debug("initializing kernel vmm");
  kernelVMM.initialize((uintptr_t*) kernelPageMap, kernelHeapStart, {.userAccessible=false, .writeable=true, .executable=true});

  log::debug("initializing kernel heap");
  kernelHeap.initialize(&kernelVMM);

  log::info("Successfully initialize Memory.");
}


auto kernel::malloc(size_t size) -> void* {
  return kernelHeap.alloc(size);
}

auto kernel::free(void* address) -> void {
  return kernelHeap.free(address);
}

auto kernel::getHeapAllocator() -> HeapAllocator& {
  return kernelHeap;
}

auto kernel::getKernelVMM() -> VMM& {
  return kernelVMM;
}
