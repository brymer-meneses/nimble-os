#include <tests/prelude.h>
#include <kernel/memory/memory.h>
#include <kernel/arch/arch.h>
#include <kernel/memory/heap_allocator.h>
#include <lib/syslib/linkedList.h>
#include <lib/syslib/allocator.h>
#include <lib/syslib/math.h>

TEST(SystemLibrary, LinkedList) {
  auto* allocator = kernel::getHeapAllocator();
  auto list = sl::LinkedList<int, HeapAllocator>(allocator);

  const auto size = 1000;

  for (size_t i = 0; i < size; i++) {
    list.append(i);
  }

  for (size_t i = 0; i < size; i++) {
    assertEq(list[i].value(), i);
  }
  list.free();
}

TEST(SystemLibrary, LinkedListIterator) {
  auto* allocator = kernel::getHeapAllocator();
  auto list = sl::LinkedList<int, HeapAllocator>(allocator);

  for (size_t i = 0; i < 1000; i++) {
    list.append(i);
  }

  int i = 0;
  for (auto elem : list) {
    assertEq(elem, i++);
  }

  list.free();
}
