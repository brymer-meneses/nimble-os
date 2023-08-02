#include "vmm.h"

#include <limine.h>
#include <kernel/utils/panic.h>

static volatile auto hhdmRequest = limine_hhdm_request {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

static u64 hhdmOffset = 0;

struct VMMFlags {
  bool isWritable;
  bool isExecutable;
  bool isUserAccessible;

  static constexpr u64 PT_FLAG_WRITEABLE  = 1ULL << 1;
  static constexpr u64 PT_FLAG_USER       = 1ULL << 2;
  static constexpr u64 PT_FLAG_EXECUTABLE = 1ULL << 63;

  constexpr auto toX86_64Flags() -> u64 {
    u64 value = 0;

    if (isWritable) {
      value |= PT_FLAG_WRITEABLE;
    }
    if (isUserAccessible) {
      value |= PT_FLAG_USER;
    }
    if (isExecutable) {
      value |= PT_FLAG_EXECUTABLE;
    }

    return value;
  }
};

struct PageTableEntry {
  private:
    u64 value;

  public:
    auto is_present() -> bool {
    };



};


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

