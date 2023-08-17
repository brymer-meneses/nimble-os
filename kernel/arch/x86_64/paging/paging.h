#pragma once
#include <cstddef>
#include <cstdint>
#include <lib/types.h>
#include <kernel/memory/vmm.h>

namespace x86_64::Paging {
  static constexpr u64 PAGE_SIZE = 0x1000;
  auto map(uintptr_t virtualAddress, uintptr_t physicalAddress, VMFlag flags) -> void;
  auto unmap(uintptr_t virtualAddrss) -> void;

  auto initialize() -> void;
}

