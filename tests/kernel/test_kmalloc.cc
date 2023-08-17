#include <tests/prelude.h>
#include <kernel/memory/memory.h>

TEST(Heap, KernelMalloc) {

  auto* p1 = (u64*) Kernel::malloc(8);

  *p1 = 64;

  auto* p2 = (u64*) Kernel::malloc(69);
  *p2 = 103;

  assertEq(*p1, 64);
  assertEq(*p2, 103);

  assertNeq(p1, p2);

  Kernel::free(p1);
  Kernel::free(p2);
}
