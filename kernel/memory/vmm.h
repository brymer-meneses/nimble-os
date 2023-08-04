#pragma once

#include <lib/types.h>
#include <optional>

#include "address.h"

struct VMMFlags {
  bool isWriteable;
  bool isExecutable;
  bool isUserAccessible;
};

namespace VMM {
  auto initialize() -> void;
  auto virtualToPhysical(const u64 virtualAddress) -> u64;
  auto physicalToVirtual(const u64 physicalAddress) -> u64;

  auto alloc(size_t length, const VMMFlags& flags) -> void*;

  auto mapPage(const PhysicalAddress physicalAddress, const VirtualAddress virtualAddress, const VMMFlags flags) -> void;
  auto unmapPage(const PhysicalAddress physicalAddress, const VirtualAddress virtualAddress, const VMMFlags flags) -> void;
  auto exploreAddress(const VirtualAddress address) -> void;
}
