#pragma once

#include <lib/types.h>
#include <optional>

struct VMFlag {
  bool userAccessible;
  bool writeable;
  bool executable;
};

namespace VMM {
  auto initialize() -> void;
  auto subHHDM(const u64 virtualAddress) -> u64;
  auto addHHDM(const u64 physicalAddress) -> u64;

  auto alloc(size_t length, VMFlag flags) -> void*;
  auto free(uintptr_t address, size_t length) -> void;

  auto map(uintptr_t virtualAddr, uintptr_t physicalAddr, VMFlag flags) -> void;
  auto unmap(uintptr_t virtualAddr) -> void;
}
