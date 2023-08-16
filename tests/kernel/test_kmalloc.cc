#include <tests/prelude.h>
#include <kernel/memory/memory.h>

TEST(Heap, KernelMalloc) {
  auto* p1 = (u64*) Kernel::malloc(8);

  assert(p1 != nullptr);
  *p1 = 64;

  auto* p2 = (u64*) Kernel::malloc(8);
  *p2 = 64;

  Kernel::free(p1);
  Kernel::free(p2);
}
