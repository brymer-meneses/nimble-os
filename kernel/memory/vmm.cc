#include <limine.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <lib/math.h>
#include <lib/bit.h>

#include "vmm.h"
#include "pmm.h"

static volatile auto hhdmRequest = limine_hhdm_request {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0,
};

static u64 hhdmOffset = 0;

static auto readCR3() -> u64 {
  u64 value;
  asm ("movq %%cr3, %0" : "=r"(value));
  return value;
}

auto VMM::physicalToVirtual(u64 physicalAddress) -> u64 {
  if (physicalAddress <= hhdmOffset) {
    return physicalAddress;
  } else {
    return physicalAddress + hhdmOffset;
  }
}

auto VMM::virtualToPhysical(u64 virtualAddress) -> u64 {
  if (virtualAddress <= hhdmOffset) {
    return virtualAddress;
  } else {
    return virtualAddress - hhdmOffset;
  }
}

auto VMM::initialize() -> void {
  if (!hhdmRequest.response) {
    Kernel::panic("HHDM Request is null");
  }
  hhdmOffset = hhdmRequest.response->offset;
}

auto VMM::exploreAddress(VirtualAddress address) -> void {

}

auto VMM::mapPage(const PhysicalAddress physicalAddress, const VirtualAddress virtualAddress, const VMMFlags flags) -> void {

}

auto VMM::unmapPage(const PhysicalAddress physicalAddress, const VirtualAddress virtualAddress, const VMMFlags flags) -> void {
  auto root = readCR3();
}

auto VMM::alloc(size_t length, const VMMFlags& flags) -> void* {

}
  

