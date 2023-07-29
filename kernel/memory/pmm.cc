#include "pmm.h"

#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <lib/thirdparty/limine.h>
#include <lib/types.h>
#include <lib/math.h>

#include <cstddef>

#include "memory_map.h"
#include "bitmap_allocator.h"


auto PMM::initialize() -> void {


  auto& memoryMap = MemoryMap::get();
  memoryMap.get();

  auto allocator = BitmapAllocator();
  PhysicalAddress address = allocator.allocateContiguousPages(10).value();

  Kernel::println("{hex}", address);

  allocator.freeContiguousPages(address, 10);
  allocator.printInfo();
}

