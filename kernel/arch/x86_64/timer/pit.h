#pragma once
#include <lib/types.h>

namespace x86_64::PIT {
  auto initialize() -> void;
  auto setPeriodic(u16) -> void;
}

