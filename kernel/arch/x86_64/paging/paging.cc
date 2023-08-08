#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/memory/memory.h>

#include <kernel/utils/assert.h>

#include <kernel/arch/x86_64/interrupt/interrupt.h>
#include <kernel/arch/cpu.h>

#include <lib/bit.h>

#include <array>

#include "paging.h"

using namespace x86_64;
using Memory::PAGE_SIZE;
using Interrupt::InterruptFrame;

static auto pageFaultHandler(InterruptFrame* frame) -> void {
  Kernel::println("Exception: Page Fault");
  Kernel::println("Accessed Address: {hex}", CPU::readCR2());
  Kernel::println("Error Code: {hex}", frame->error_code);
  Kernel::halt();
}

static auto genPageFaultHandler(InterruptFrame* frame) -> void {
  Kernel::println("Exception: General Page Fault");
  Kernel::println("Accessed Address: {hex}", CPU::readCR2());
  Kernel::println("Error Code: {hex}", frame->error_code);
  Kernel::halt();
}

static auto invalidateTLBCache(uintptr_t address) -> void {
  asm volatile ("invlpg %0" :: "m"(address));
}

static constexpr u64 PTE_PRESENT = 1ull << 0;
static constexpr u64 PTE_WRITEABLE = 1ull << 1;
static constexpr u64 PTE_USER_ACCESSIBLE = 1ull << 2;
static constexpr u64 PTE_NOT_EXECUTABLE = 1ull << 63;
static constexpr u64 PTE_ADDRESS_MASK = 0x000ffffffffff000;

static auto getNextLevel(uintptr_t* topLevel, size_t index, VMFlag flags, bool shouldAllocate) -> uintptr_t* {
  auto entry = topLevel[index];
  if ((entry & PTE_PRESENT) != 0) {
    return (uintptr_t*) VMM::physicalToVirtual(entry & PTE_ADDRESS_MASK);
  }

  if (!shouldAllocate) {
    return nullptr;
  }

  const auto page = (uintptr_t) PMM::allocatePage();
  Kernel::assert(page != 0, "allocated page is a nullptr");
  Kernel::assert(page % PAGE_SIZE == 0, "allocated page must be page-aligned");

  std::memset((void*) VMM::physicalToVirtual(page), 0, PAGE_SIZE);

  auto newLevel = page;
  newLevel |= PTE_PRESENT;
  newLevel |= flags.writeable ? PTE_WRITEABLE : 0;
  newLevel |= flags.userAccessible ? PTE_USER_ACCESSIBLE : 0;
  newLevel |= !flags.executable ? PTE_NOT_EXECUTABLE : 0;

  topLevel[index] = newLevel;

  // can't believe i spent a couple of days not being able to figure out that
  // that I am supposed to mask the bits here
  return (uintptr_t*) VMM::physicalToVirtual(newLevel & PTE_ADDRESS_MASK);
}

auto Paging::map(uintptr_t virtualAddress, uintptr_t physicalAddress, VMFlag vmflags) -> void {
  Kernel::assert(virtualAddress % Memory::PAGE_SIZE == 0, "virtual address should be page-aligned!");
  Kernel::assert(physicalAddress % Memory::PAGE_SIZE == 0, "physical address should be page-aligned!");

  std::memset((void*) VMM::physicalToVirtual(physicalAddress), 0, PAGE_SIZE);

  auto pml4Index = (virtualAddress >> 39) & 0x1ff;
  auto pdpIndex = (virtualAddress >> 30) & 0x1ff;
  auto pdIndex = (virtualAddress >> 21) & 0x1ff;
  auto ptIndex = (virtualAddress >> 12) & 0x1ff;

  auto* pml4 = (uintptr_t*) VMM::physicalToVirtual(CPU::readCR3() & PTE_ADDRESS_MASK);
  auto* pdp = getNextLevel(pml4, pml4Index, vmflags, true);
  auto* pd = getNextLevel(pdp, pdpIndex, vmflags, true);
  auto* pt = getNextLevel(pd, pdIndex, vmflags, true);

  auto entry = physicalAddress;
  entry |= PTE_PRESENT;
  entry |= vmflags.writeable ? PTE_WRITEABLE : 0;
  entry |= !vmflags.executable ? PTE_NOT_EXECUTABLE : 0;
  entry |= vmflags.userAccessible ? PTE_USER_ACCESSIBLE : 0;

  pt[ptIndex] = entry;
  invalidateTLBCache(virtualAddress);
}


auto Paging::unmap(uintptr_t virtualAddress) -> void {
  Kernel::assert(virtualAddress % Memory::PAGE_SIZE == 0, "virtual address should be page-aligned!");

  auto pml4Index = (virtualAddress >> 39) & 0x1ff;
  auto pdpIndex = (virtualAddress >> 30) & 0x1ff;
  auto pdIndex = (virtualAddress >> 21) & 0x1ff;
  auto ptIndex = (virtualAddress >> 12) & 0x1ff;

  VMFlag flags;

  auto* pml4 = (uintptr_t*) VMM::physicalToVirtual(CPU::readCR3() & PTE_ADDRESS_MASK);
  auto* pdp = getNextLevel(pml4, pml4Index, flags, false);
  auto* pd = getNextLevel(pdp, pdpIndex, flags, false);
  auto* pt = getNextLevel(pd, pdIndex, flags, false);

  pt[ptIndex] = 0;
  invalidateTLBCache(virtualAddress);
}

auto Paging::initialize() -> void {
  Interrupt::setExceptionHandler(0xE, pageFaultHandler);
  Interrupt::setExceptionHandler(0xD, genPageFaultHandler);
}
