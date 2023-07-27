#include "pmm.h"

#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <lib/thirdparty/limine.h>
#include <lib/types.h>
#include <lib/math.h>

#include <cstddef>

#include "memory_map.h"


auto PMM::initialize() -> void {

  auto& memoryMap = MemoryMap::get();
  memoryMap.get();

  // auto allocator = BitmapAllocator();
  // u64 address = allocator.allocatePage().value();
  // u64 address1 = allocator.allocatePage().value();
  //
  // Kernel::println("{hex}", address);
  //
  // allocator.freePage(address);
  // allocator.freePage(address1);
  //
  // Kernel::println("yoo {bin}", (u8) allocator.mBitmapData[0]);

}

