#include "bitmap_allocator.h"
#include "pmm.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
#include <lib/math.h>
#include <algorithm>

#include "memory_map.h"

using PMM::PAGE_SIZE;

#define BITMAP_FREE 0
#define BITMAP_USED 1

using Bitmap = BitmapAllocator::Bitmap;

auto Bitmap::setFree(size_t index) -> void {
  auto row = index / 8;
  auto col = index % 8;
  data[row] = data[row] & ~(1 << col);
  usedPages -= 1;
}

auto Bitmap::setUsed(size_t index) -> void {
  auto row = index / 8;
  auto col = index % 8;
  data[row] = data[row] | (1 << col);
  usedPages += 1;
}

auto Bitmap::freeAll() -> void {
  for (size_t i=0; i<maxPages; i++) {
    data[i] = 0;
  }
  usedPages = 0;
}

auto Bitmap::isPageFree(size_t index) -> bool {
  auto row = index / 8;
  auto col = index % 8;
  return (data[row] & (1 << col)) == 0;
}

auto Bitmap::setContiguousPagesAsUsed(size_t pages) {
  for (size_t i = 0; i < pages; i++) {
    setUsed(i);
  }
}

Bitmap::Bitmap(u8* data, size_t maxPages) : data(data), maxPages(maxPages)  {
  const auto dataSize = Math::ceilDiv(maxPages, 8);
  const auto pagesAllocateToBitmap = Math::ceilDiv(dataSize, PAGE_SIZE);
  freeAll();
  setContiguousPagesAsUsed(pagesAllocateToBitmap);
};


BitmapAllocator::BitmapAllocator() { 

  // I'm an idiot, it took me a whole day to figure out this computation.
  const auto totalPages = memoryMap.usablePages;
  const auto bitmapSize = Math::ceilDiv(totalPages, 8);

  // find a place for the bitmap
  for (size_t i = memoryMap.usable.start; i <= memoryMap.usable.end; i++) {
    auto* entry = memoryMap[i];
    if (entry->length >= bitmapSize) {
      bitmap = Bitmap(reinterpret_cast<u8*>(entry->base), bitmapSize);
      break;
    }
  }

  // ensure that bitmap.data is not a nullptr
  KERNEL_ASSERT(bitmap.data);

  // the bitmap marks the first `n` pages as used since this is occuped by the
  // bitmap data itself
  lastIndexUsed = bitmap.usedPages + 1;
}

auto BitmapAllocator::getCurrentEntry() -> limine_memmap_entry* {

  auto* entry = memoryMap[entryIndex];
  const auto canEntryFit = entry->length >= entryPageIndex * PAGE_SIZE;
  if (not canEntryFit) {
    entryIndex += 1;
    if (entryIndex > memoryMap.usable.end) {
      Kernel::panic("Failed to allocate a page since memory is full");
    }

    // get the new entry
    entry = memoryMap[entryPageIndex];
    // reset the `entryPageIndex` since this is relative to the current entry
    entryPageIndex = 0;
  }
  
  return entry;
}

auto BitmapAllocator::allocatePage() -> std::optional<PhysicalAddress> {

  while (bitmap.usedPages < bitmap.maxPages) {

    if (lastIndexUsed > bitmap.usedPages) {
      // reset the lastIndexUsed just after the reserved pages for the bitmap
      lastIndexUsed = bitmap.usedPages + 1;
    }

    if (bitmap.isPageFree(lastIndexUsed)) {
      auto address = getAddressFromBitmapIndex(lastIndexUsed);
      bitmap.setUsed(lastIndexUsed);
      lastIndexUsed += 1;
      entryPageIndex += 1;
      return address;
    } 

    lastIndexUsed += 1;
  }

  return std::nullopt;
}

auto BitmapAllocator::allocateContiguousPages(size_t requiredPages) -> std::optional<PhysicalAddress> {

  if (requiredPages > bitmap.maxPages) {
    Kernel::panic("Can't allocate that much memory");
  }

  size_t numPages = 0;
  size_t index = lastIndexUsed;
  size_t baseIndex = index;
  size_t baseEntryIndex = entryIndex;

  while (numPages < requiredPages) {

    if (index > bitmap.maxPages) {
      break;
    }

    if (bitmap.isPageFree(index)) {
      numPages += 1;
    } else {
      baseIndex = index;
      numPages = 0;
    }

    index += 1;
  }

  const auto achievedRequiredPages = numPages == requiredPages;
  const auto isSameEntry = baseEntryIndex == entryIndex;

  if (!achievedRequiredPages or !isSameEntry) {
    return std::nullopt;
  }

  lastIndexUsed = index;
  auto address = getAddressFromBitmapIndex(baseIndex);
  bitmap.setContiguousPagesAsUsed(requiredPages);
  return address;
}

auto BitmapAllocator::getAddressFromBitmapIndex(size_t index) -> std::optional<PhysicalAddress> {

  for (size_t i = memoryMap.usable.start; i <= memoryMap.usable.end; i++) {
    auto* entry = memoryMap[i];

    const auto pagesInEntry = entry->length / PAGE_SIZE;

    if (index > pagesInEntry) {
      index -= pagesInEntry;
      continue;
    }

    // we found the entry we're looking for
    return entry->base + index * PAGE_SIZE;
  }

  return std::nullopt;
}

auto BitmapAllocator::getBitmapIndexFromAddress(PhysicalAddress address) -> std::optional<size_t> {
  limine_memmap_entry* entry = nullptr;
  for (size_t i = memoryMap.usable.start; i <= memoryMap.usable.end; i++) {
    entry = memoryMap[i];
    auto isAddressInEntry = address >= entry->base && address < entry->base + entry->length;

    if (isAddressInEntry) {
      break;
    }
  }

  if (entry == nullptr)  {
    return std::nullopt;
  } else {
    return (address - entry->base) / PAGE_SIZE;
  }
}

auto BitmapAllocator::freePage(PhysicalAddress address) -> void {
  const auto index = getBitmapIndexFromAddress(address);

  if (!index) {
    Kernel::panic("Failed to free page that is out of range");
  }

  bitmap.setFree(index.value());
}

auto BitmapAllocator::getBitmapData(size_t i) -> u8 {
  return bitmap.data[i];
}
