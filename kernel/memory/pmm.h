#pragma once

#include <lib/types.h>

namespace PMM {


  auto initialize() -> void;

  [[nodiscard]] auto allocatePage() -> void*;
  [[nodiscard]] auto allocateContiguousPages(size_t num) -> void*;
  
  auto freePage(uintptr_t address) -> void;
  auto freeContiguousPages(uintptr_t address, size_t num) -> void;
}
