#include <kernel/memory/pmm.h>
#include <kernel/memory/memory.h>

#include <kernel/utils/assert.h>
#include <kernel/utils/logger.h>

#include <kernel/arch/x86_64/interrupt/interrupt.h>
#include <kernel/arch/cpu.h>

#include "paging.h"

using namespace x86_64;
using paging::PAGE_SIZE;

static auto pageFaultHandler(arch::cpu::Context* frame) -> void {
  serial::println("Exception: Page Fault");
  serial::println("Accessed Address: {#0x16}", arch::cpu::readCR2());
  serial::println("Error Code: {#0x16}", frame->error_code);
  kernel::halt();
}
static auto genPageFaultHandler(arch::cpu::Context* frame) -> void {
  serial::println("Exception: General Page Fault");
  serial::println("Accessed Address: {#0x16}", arch::cpu::readCR2());
  serial::println("Error Code: {#0x16}", frame->error_code);
  kernel::halt();
}

static auto invalidateTLBCache(uintptr_t address) -> void {
  asm volatile ("invlpg %0" :: "m"(address));
}

static constexpr u64 PTE_PRESENT = 1ull << 0;
static constexpr u64 PTE_WRITEABLE = 1ull << 1;
static constexpr u64 PTE_USER_ACCESSIBLE = 1ull << 2;
static constexpr u64 PTE_NOT_EXECUTABLE = 1ull << 63;
static constexpr u64 PTE_ADDRESS_MASK = 0x000ffffffffff000;


auto paging::getNextLevel(uintptr_t* topLevel, size_t index, VMFlag flags, bool shouldAllocate) -> uintptr_t* {
  auto entry = topLevel[index];
  if ((entry & PTE_PRESENT) != 0) {
    return (uintptr_t*) memory::addHHDM(entry & PTE_ADDRESS_MASK);
  }

  if (!shouldAllocate) {
    return nullptr;
  }

  const auto page = (uintptr_t) PMM::allocatePage();
  kernel::assert(page != 0, "allocated page is a nullptr");
  kernel::assert(page % PAGE_SIZE == 0, "allocated page must be page-aligned");

  std::memset((void*) memory::addHHDM(page), 0, PAGE_SIZE);

  auto newLevel = page;
  newLevel |= PTE_PRESENT;
  newLevel |= flags.writeable ? PTE_WRITEABLE : 0;
  newLevel |= flags.userAccessible ? PTE_USER_ACCESSIBLE : 0;
  newLevel |= !flags.executable ? PTE_NOT_EXECUTABLE : 0;

  topLevel[index] = newLevel;

  // can't believe i spent a couple of days not being able to figure out that
  // that I am supposed to mask the bits here
  return (uintptr_t*) memory::addHHDM(newLevel & PTE_ADDRESS_MASK);
}

auto paging::map(uintptr_t* pml4, uintptr_t virtualAddress, uintptr_t physicalAddress, VMFlag vmflags) -> void {
  kernel::assert(virtualAddress % PAGE_SIZE == 0, "virtual address should be page-aligned!");
  kernel::assert(physicalAddress % PAGE_SIZE == 0, "physical address should be page-aligned!");

  auto pml4Index = (virtualAddress >> 39) & 0x1ff;
  auto pdpIndex = (virtualAddress >> 30) & 0x1ff;
  auto pdIndex = (virtualAddress >> 21) & 0x1ff;
  auto ptIndex = (virtualAddress >> 12) & 0x1ff;

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


auto paging::unmap(uintptr_t* pml4, uintptr_t virtualAddress) -> void {
  kernel::assert(virtualAddress % PAGE_SIZE == 0, "virtual address should be page-aligned!");

  auto pml4Index = (virtualAddress >> 39) & 0x1ff;
  auto pdpIndex = (virtualAddress >> 30) & 0x1ff;
  auto pdIndex = (virtualAddress >> 21) & 0x1ff;
  auto ptIndex = (virtualAddress >> 12) & 0x1ff;

  VMFlag flags;
  auto* pdp = getNextLevel(pml4, pml4Index, flags, false);
  auto* pd = getNextLevel(pdp, pdpIndex, flags, false);
  auto* pt = getNextLevel(pd, pdIndex, flags, false);

  PMM::freePage(memory::addHHDM((uintptr_t) &pt[ptIndex]));
  pt[ptIndex] = 0;

  invalidateTLBCache(virtualAddress);
}

extern uintptr_t text_start_addr, text_end_addr;
extern uintptr_t rodata_start_addr, rodata_end_addr;
extern uintptr_t data_start_addr, data_end_addr;

auto paging::initialize() -> void {
  interrupt::setInterruptHandler(0xE, pageFaultHandler);
  interrupt::setInterruptHandler(0xD, genPageFaultHandler);

  log::info("Initialized Paging");
}

auto paging::getInitialPageMap() -> uintptr_t* {
  return (uintptr_t*) memory::addHHDM(arch::cpu::readCR3() & PTE_ADDRESS_MASK);
}

auto paging::switchPageMap(uintptr_t* root) -> void {
  asm volatile (
    "mov %0, %%cr3"
    : 
    : "r" (memory::subHHDM((uintptr_t) root))
    : "memory"
  );
}
