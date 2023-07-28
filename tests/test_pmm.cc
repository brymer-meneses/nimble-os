
#include <tests/prelude.h>
#include <kernel/memory/bitmap_allocator.h>


TEST(BitmapAllocator, AllocateAndFreePage) {

  // TODO: 
  // make this test pass
  auto allocator = BitmapAllocator();

  u64 address = allocator.allocatePage().value();
  u64 address1 = allocator.allocatePage().value();

  *((u64*) address1) = 5;

  assertEq(*((u64*) address1), (u64) 5);
  assertEq(allocator.getBitmapData(0), (u8) 0b11);

  allocator.freePage(address);
  allocator.freePage(address1);

  assertEq(allocator.getBitmapData(0), (u8) 0b00);
}
