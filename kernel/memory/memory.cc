#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

#include <limine.h>

#include "memory.h"
#include "memory_map.h"
#include "pmm.h"

auto Memory::initialize() -> void {
  PMM::initialize();
  VMM::initialize();

  // Kernel::println("{hex}", MemoryMap::get().usable.count);
  // for (const auto* entry : MemoryMap::get().usable) {
  //   Kernel::println("{hex}", entry->base);
  // }

  for (const auto* entry : MemoryMap::get()) {
    Kernel::println("{hex} {}", entry->base, entry->type);
  }
}

