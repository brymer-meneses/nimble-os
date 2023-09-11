#pragma once
#include <cstdint>
#include <lib/types.h>
#include <kernel/arch/cpu.h>

namespace x86_64::interrupt {

// Note: As much as i want this to inherit from `Format::FormatArgument`
// so we can trivially print it using `Kernel::println`, it kinda messes up the
// placement if we did so.

  using InterruptHandler = void (*)(arch::cpu::Context*);

  auto setInterruptHandler(const u16 interruptNumber, InterruptHandler handler) -> void;
  auto sendEOI(const u8 interruptNumber) -> void;

  auto initialize() -> void;

  extern inline auto disable() -> void {
    asm volatile ("cli");
  }

  extern inline auto enable() -> void {
    asm volatile ("sti");
  }

}
