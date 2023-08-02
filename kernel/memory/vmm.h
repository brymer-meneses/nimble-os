#pragma once

#include <lib/types.h>
#include <optional>

#include "address.h"

namespace VMM {
  auto initialize() -> void;
  auto allocate(size_t length, size_t flags) -> std::optional<VirtualAddress>;
  auto free(VirtualAddress address) -> void;

  auto virtualToPhysical(u64 virtualAddress) -> u64;
  auto physicalToVirtual(u64 physicalAddress) -> u64;
}
