
#include <tests/prelude.h>
#include <kernel/memory/pmm.h>


TEST(PMM, AllocatePage) {

  u64 address = PMM::allocatePage();
  u64 address1 = PMM::allocatePage();

  *((u64*) address1) = 5;

  assertEq(*((u64*) address1), (u64) 5);

  PMM::freePage(address);
  PMM::freePage(address1);
}
