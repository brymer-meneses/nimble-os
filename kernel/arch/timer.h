#pragma once

#include <lib/types.h>
#include <kernel/arch/arch.h>

namespace arch::timer {
  auto initialize() -> void;
  auto wait(u64 nanos) -> void;

  auto setInterruptCallback(arch::interrupt::InterruptHandler handler, u64 nanos) -> void;
  auto setPeriodic(u64 nanos) -> void;
}
