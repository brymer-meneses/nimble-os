#include <lib/vector.h>
#include <lib/allocator.h>
#include <tests/prelude.h>
#include <kernel/memory/memory.h>

TEST(SystemLibrary, Vector) {

  auto& allocator = Kernel::getHeapAllocator();
  using KernelAllocator = decltype(allocator);

  auto vector = sl::Vector<int, KernelAllocator>(allocator, 1);

  vector.push(1);
  vector.push(2);
  vector.push(3);

  assertEq(vector[0], 1);
  assertEq(vector[1], 2);
  assertEq(vector[2], 3);

  vector.free();
}
