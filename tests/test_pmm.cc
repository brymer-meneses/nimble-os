
#include <tests/prelude.h>
#include <kernel/memory/bitmap_allocator.h>

TEST(BitmapAllocator, AllocateAndFreePage) {

  auto allocator = BitmapAllocator();

  u64 address = allocator.allocatePage().value();
  u64 address1 = allocator.allocatePage().value();

  assertEq(allocator[0], (u8) 0b11);

  allocator.freePage(address);
  allocator.freePage(address1);

  assertEq(allocator[1], (u8) 0b00);
}
