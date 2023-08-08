#include <limine.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <lib/math.h>
#include <lib/bit.h>

#include "vmm.h"
#include "pmm.h"
#include "memory.h"

#include <kernel/arch/platform.h>
#include <tuple>

static volatile auto hhdmRequest = limine_hhdm_request {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0,
};

static u64 hhdmOffset = 0;

auto VMM::physicalToVirtual(u64 physicalAddress) -> u64 {
  if (hhdmOffset) [[likely]] {
    return physicalAddress + hhdmOffset;
  } 
  Kernel::panic("VMM not initialized");
  __builtin_unreachable();
}

auto VMM::virtualToPhysical(u64 virtualAddress) -> u64 {
  if (hhdmOffset) [[likely]] {
    return virtualAddress - hhdmOffset;
  }
  Kernel::panic("VMM not initialized");
  __builtin_unreachable();
}

auto VMM::initialize() -> void {
  if (!hhdmRequest.response) {
    Kernel::panic("HHDM Request is null");
  }
  hhdmOffset = hhdmRequest.response->offset;

  VMFlag flags;
  flags.executable = true;
  flags.userAccessible = true;
  flags.writeable = true;

  u64 virtualAddr = 0xdeadc0de000;
  u64 physicalAddr = (uintptr_t) PMM::allocatePage();
  Arch::Paging::map(virtualAddr, physicalAddr, flags);

  *((u64*) virtualAddr) = 5;
  // dump_pagetable(virtualAddr);
}


