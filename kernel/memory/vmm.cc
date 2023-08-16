#include <limine.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <lib/math.h>
#include <lib/bit.h>

#include "vmm.h"
#include "pmm.h"
#include "memory.h"

#include <kernel/arch/platform.h>

using Memory::PAGE_SIZE;

static volatile auto hhdmRequest = limine_hhdm_request {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0,
};

static u64 hhdmOffset = 0;

auto VMM::addHHDM(u64 physicalAddress) -> u64 {
  Kernel::assert(hhdmOffset, "VMM not initialized");

  return physicalAddress + hhdmOffset;
}

auto VMM::subHHDM(u64 virtualAddress) -> u64 {
  Kernel::assert(hhdmOffset, "VMM not initialized");

  return virtualAddress - hhdmOffset;
}

auto VMM::initialize() -> void {
  if (!hhdmRequest.response) {
    Kernel::panic("HHDM Request is null");
  }
  hhdmOffset = hhdmRequest.response->offset;
}

auto VMM::map(uintptr_t virtualAddr, uintptr_t physicalAddr, VMFlag flags) -> void {
  Arch::Paging::map(virtualAddr, physicalAddr, flags);
}
auto VMM::unmap(uintptr_t virtualAddr) -> void {
  Arch::Paging::unmap(virtualAddr);
}

struct BumpAllocator {

private:
  size_t mCurrent = 0;
  size_t mBase = 0;

public:
  BumpAllocator(uintptr_t base) : mBase(base) {}

  auto allocate(size_t length, VMFlag flags) -> uintptr_t {
    const auto pages = Math::ceilDiv(length, PAGE_SIZE);
    const auto address = mBase + mCurrent * PAGE_SIZE;

    for (size_t i = 0; i < pages; i++) {
      auto page = (uintptr_t) PMM::allocatePage();
      std::memset((void*) VMM::addHHDM(page), 0, PAGE_SIZE);
      VMM::map(address, page, flags);
    }

    mCurrent += pages;
    return address;
  }

  auto free(size_t length, uintptr_t address) -> void {
    const auto pages = Math::ceilDiv(length, PAGE_SIZE);
    Kernel::assert(mCurrent - pages >= 0, "tried to free with length surpassing the amount of allocated memory");

    if (address != mBase + (mCurrent - pages) * PAGE_SIZE) {
      return;
    }

    for (size_t i = 0; i < pages; i++) {
      VMM::unmap(mBase + (mCurrent - pages + i) * PAGE_SIZE);
    }

    mCurrent -= pages;
  }
};

static BumpAllocator gAllocator{ 4 * 1024 * 1024 };

auto VMM::alloc(size_t length, VMFlag flags) -> void* {
  return (void*) (uintptr_t) gAllocator.allocate(length, flags);
}

auto VMM::free(uintptr_t addr, size_t length) -> void {
  gAllocator.free(length, addr);
}

