#include "bitmap_allocator.h"
#include "pmm.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
#include <lib/math.h>

#include "usable_memory.h"

using PMM::PAGE_SIZE;


BitmapAllocator::BitmapAllocator() {

  static volatile auto memmapRequest = limine_memmap_request {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
  };

  static volatile auto hhdmRequest = limine_hhdm_request {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
  };

  mMemmap = memmapRequest.response;
  mHhdm = hhdmRequest.response;
  mMemory = UsableMemory(mMemmap);

  // the bitmap is a u8 array, each `u8` corresponds to 8 pages
  mBitmapSize = Math::ceilDiv(mMemory.usablePages, 8);

  // find a place for the bitmap
  for (size_t i=0; i<mMemory.usableEntries; i++) {

    auto* entry = mMemory[i].value();

    if (entry->length >= mBitmapSize) {
      mBitmapData = (u8*) entry->base;
      entry->base += mBitmapSize;
      entry->length -= mBitmapSize;
      break;
    }
  }

  if (!mBitmapData) {
    Kernel::panic("Cannot find a place for `mBitmapData`");
  }

  freeAll();
}

auto BitmapAllocator::setFree(size_t i) -> void {
  auto row = i / 8;
  auto col = i % 8;
  mBitmapData[row] = mBitmapData[row] & ~(1 << col);
}

auto BitmapAllocator::setUsed(size_t i) -> void {
  auto row = i / 8;
  auto col = i % 8;
  mBitmapData[row] = mBitmapData[row] | (1 << col);
}

auto BitmapAllocator::freeAll() -> void {
  for (size_t i=0; i<mBitmapSize; i++) {
    mBitmapData[i] = 0;
  }
}

auto BitmapAllocator::isBitFree(size_t i) -> bool {
  auto row = i / 8;
  auto col = i % 8;
  return (mBitmapData[row] & (1 << col)) == 0;
}

auto BitmapAllocator::allocatePage() -> std::optional<PhysicalAddress> {
  static size_t i = 0; 
  static u64 lastUsedIndex = 0;

  while (lastUsedIndex < mMemory.usablePages) {

    // find which memmap entry should be used
    auto* entry = mMemory[i].value();
    while (lastUsedIndex * PAGE_SIZE >= entry->base + entry->length) {

      if (!mMemory[i]) {
        Kernel::panic("Memory full");
        return std::nullopt;
      }

      entry = mMemory[i].value();
      i += 1;
    }

    if (isBitFree(lastUsedIndex)) {
      uintptr_t address = entry->base + lastUsedIndex * PAGE_SIZE;

      setUsed(lastUsedIndex);
      lastUsedIndex += 1;
      return address;
    }

    lastUsedIndex += 1;
  }

  return std::nullopt;
}

auto BitmapAllocator::freePage(PhysicalAddress address) -> void {
  limine_memmap_entry* entry = nullptr;

  for (size_t i = 0; i < mMemory.usableEntries; i++) {
    entry = mMemory[i].value();
    if (address >= entry->base && address < entry->base + entry->length) {
      break;
    }
  }

  // Kernel::println("Freeing {hex}", (address-entry->base)/PAGE_SIZE);
  setFree((address - entry->base) / PAGE_SIZE);
}

auto BitmapAllocator::operator[](size_t i) -> u8 {
  return mBitmapData[i];
}
