#include <tests/prelude.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/heap_allocator.h>
#include <lib/syslib/vector.h>
#include <lib/syslib/allocator.h>

TEST(SystemLibrary, Vector) {

  auto& allocator = Kernel::getHeapAllocator();
  auto vector = sl::Vector<int, HeapAllocator>(allocator, 1);

  for (int i = 0; i < 1000; i++) {
    vector.push(i);
    assertEq(vector[i], i);
  }

  // vector.free();
}

// TODO: Currently broken :<
// TEST(SystemLibrary, VectorIterator) {
//   auto& allocator = Kernel::getHeapAllocator();
//   auto vector = sl::Vector<int, HeapAllocator>(allocator, 1);
//
//   for (int i = 0; i < 1000; i++) {
//     vector.push(i);
//   }
//
//   int i = 0;
//   for (const auto elem : vector) {
//     assertEq(elem, i++);
//   }
//
//   // vector.free();
// }
