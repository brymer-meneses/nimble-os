#pragma once

#include <lib/types.h>
#include "address.h"

namespace PMM {

  constexpr u64 PAGE_SIZE = 0x1000;

  auto initialize() -> void;

  [[nodiscard]] auto allocatePage() -> PhysicalAddress;
  [[nodiscard]] auto allocateContiguousPages(size_t num) -> PhysicalAddress;
  
  auto freePage(PhysicalAddress address) -> void;
  auto freeContiguousPages(PhysicalAddress address, size_t num) -> void;
}
