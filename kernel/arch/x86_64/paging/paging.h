#pragma once
#include <cstddef>
#include <cstdint>
#include <lib/types.h>
#include <kernel/memory/vmm.h>

namespace x86_64::Paging {
  auto map(uintptr_t virtualAddress, uintptr_t physicalAddress, VMFlag flags) -> void;
  auto unmap(uintptr_t virtualAddrss) -> void;

  auto initialize() -> void;
}

