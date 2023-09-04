#pragma once
#include <lib/types.h>

namespace x86_64::PIT {
  auto setPeriodic(u64) -> void;
  auto read() -> u64;
  auto wait(u64 nanos) -> void;
}

