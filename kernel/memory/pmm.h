#pragma once

#include <lib/types.h>
#include "address.h"

namespace PMM {


  auto initialize() -> void;

  [[nodiscard]] auto allocatePage() -> PhysicalAddress;
  [[nodiscard]] auto allocateContiguousPages(size_t num) -> PhysicalAddress;
  
  auto freePage(PhysicalAddress address) -> void;
  auto freeContiguousPages(PhysicalAddress address, size_t num) -> void;
}
