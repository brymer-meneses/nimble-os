#include <tests/prelude.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/heap_allocator.h>
#include <lib/syslib/linkedList.h>
#include <lib/syslib/allocator.h>

// TODO: freeing linked list results to segfault somehow :<

TEST(SystemLibrary, LinkedList) {
  auto& allocator = Kernel::getHeapAllocator();
  auto list = sl::LinkedList<int, HeapAllocator>(allocator);

  for (size_t i = 0; i < 40; i++) {
    list.append(i);
  }

  for (size_t i = 0; i < 40; i++) {
    assertEq(list[i], i);
  }
}

TEST(SystemLibrary, LinkedListIterator) {
  auto& allocator = Kernel::getHeapAllocator();
  auto list = sl::LinkedList<int, HeapAllocator>(allocator);

  for (size_t i = 0; i < 40; i++) {
    list.append(i);
  }

  int i = 0;
  for (auto elem : list) {
    assertEq(elem, i++);
  }
}
