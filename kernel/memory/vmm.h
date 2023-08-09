#pragma once

#include <lib/types.h>
#include <optional>

#include "address.h"
#include "memory.h"

struct VMFlag {
  bool userAccessible;
  bool writeable;
  bool executable;
};

namespace VMM {
  auto initialize() -> void;
  auto virtualToPhysical(const u64 virtualAddress) -> u64;
  auto physicalToVirtual(const u64 physicalAddress) -> u64;

  auto alloc(size_t length, VMFlag flags) -> void*;
  auto free(VirtualAddress address, size_t length) -> void;

  auto map(VirtualAddress virtualAddr, PhysicalAddress physicalAddr, VMFlag flags) -> void;
  auto unmap(VirtualAddress virtualAddr) -> void;
}
