#pragma once

#include <lib/types.h>
#include <optional>

#include "address.h"

struct VMFlag {
  bool userAccessible;
  bool writeable;
  bool executable;
};

namespace VMM {
  auto initialize() -> void;
  auto virtualToPhysical(const u64 virtualAddress) -> u64;
  auto physicalToVirtual(const u64 physicalAddress) -> u64;

  auto map(VirtualAddress virtualAddr, PhysicalAddress physicalAddr, VMFlag flags) -> void;
  auto unmap(VirtualAddress virtualAddr, PhysicalAddress physicalAddr) -> void;
}
