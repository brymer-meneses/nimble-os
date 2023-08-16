#include <tests/prelude.h>
#include <kernel/memory/vmm.h>

TEST(VMM, Allocation) {
  auto flags = VMFlag {
    .userAccessible = true, .writeable = true, .executable = true
  };
  auto *page = (uintptr_t *) VMM::alloc(sizeof(uintptr_t), flags);

  *(page) = 5;
  assertEq(*page, 5);
  // VMM::free((uintptr_t) page);
}
