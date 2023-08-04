#include "vmm.h"

#include <limine.h>
#include <kernel/utils/panic.h>
#include <lib/math.h>
#include <lib/bit.h>

static volatile auto hhdmRequest = limine_hhdm_request {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

static u64 hhdmOffset = 0;


auto VMM::initialize() -> void {
  if (!hhdmRequest.response) {
    Kernel::panic("HHDM Request is null");
  }
  hhdmOffset = hhdmRequest.response->offset;
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

