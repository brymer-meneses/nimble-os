#pragma once

#include <lib/types.h>

namespace PMM {


  auto initialize() -> void;

  [[nodiscard]] auto allocatePage() -> void*;
  auto freePage(uintptr_t address) -> void;
}
