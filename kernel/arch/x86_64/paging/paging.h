#pragma once
#include <cstddef>
#include <cstdint>
#include <lib/types.h>
#include <kernel/memory/vmm.h>

namespace x86_64::paging {
  static constexpr u64 PAGE_SIZE = 0x1000;

  auto map(uintptr_t* root, uintptr_t virtualAddress, uintptr_t physicalAddress, VMFlag flags) -> void;
  auto unmap(uintptr_t* root, uintptr_t virtualAddrss) -> void;

  auto initialize() -> void;

  auto getNextLevel(uintptr_t* topLevel, size_t index, VMFlag flags, bool shouldAllocate) -> uintptr_t*;
  auto getInitialPageMap() -> uintptr_t*;
  auto switchPageMap(uintptr_t* root) -> void;
}

