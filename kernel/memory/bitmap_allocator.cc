#include "bitmap_allocator.h"
#include "pmm.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
#include <lib/bit.h>
#include <lib/math.h>

#include "memory_map.h"
#include "memory.h"

using Memory::PAGE_SIZE;

#define BITMAP_FREE 0
#define BITMAP_USED 1

using Bitmap = BitmapAllocator::Bitmap;

auto Bitmap::setFree(size_t index) -> void {
  auto row = index / 8;
  auto col = index % 8;
  Bit::unsetMut(data[row], col);
  usedPages -= 1;
}

auto Bitmap::setUsed(size_t index) -> void {
  auto row = index / 8;
  auto col = index % 8;
  Bit::setMut(data[row], col);
  usedPages += 1;
}

auto Bitmap::freeAll() -> void {
  for (size_t i=0; i<maxPages/8; i++) {
    data[i] = 0;
  }
  usedPages = 0;
}

auto Bitmap::isPageFree(size_t index) -> bool {
  auto row = index / 8;
  auto col = index % 8;
  return Bit::get(data[row], col) == 0;
}

auto Bitmap::setContiguousPagesAsUsed(size_t start, size_t end) {
  for (size_t i = start; i < end; i++) {
    setUsed(i);
  }
}

Bitmap::Bitmap(u8* data, size_t offset, size_t maxPages) : data(data), maxPages(maxPages)  {
  const auto dataSize = Math::ceilDiv(maxPages, 8);
  const auto pagesAllocateToBitmap = Math::ceilDiv(dataSize, PAGE_SIZE);

  reservedPages = pagesAllocateToBitmap;

  freeAll();
  setContiguousPagesAsUsed(offset, pagesAllocateToBitmap);
};


auto BitmapAllocator::initialize() -> void { 

  // I'm an idiot, it took me a whole day to figure out this computation.
  const auto totalPages = memoryMap.usable.pages;
  const auto bitmapSize = Math::ceilDiv(totalPages, 8);

  // find a place for the bitmap
  size_t skippedPages = 0;
  for (auto* entry : memoryMap.usable) {
    if (entry->length >= bitmapSize) {
      bitmap = Bitmap(reinterpret_cast<u8*>(entry->base), skippedPages, totalPages);
      break;
    }

    // we need to keep track of the skipped pages, since some entries may not be able to fit the 
    // bitmap data
    skippedPages += entry->length / PAGE_SIZE;
  }

  // ensure that bitmap.data is not a nullptr
  Kernel::assert(bitmap.data != nullptr);

  // the bitmap marks the first `n` pages as used since this is occuped by the
  // bitmap data itself
  lastIndexUsed = skippedPages + bitmap.usedPages;
}

auto BitmapAllocator::allocatePage() -> std::optional<PhysicalAddress> {

  while (bitmap.usedPages < bitmap.maxPages) {

    if (lastIndexUsed > bitmap.usedPages) {
      // reset the lastIndexUsed just after the reserved pages for the bitmap
      lastIndexUsed = bitmap.usedPages;
    }

    if (bitmap.isPageFree(lastIndexUsed)) {
      auto address = getAddressFromBitmapIndex(lastIndexUsed);
      bitmap.setUsed(lastIndexUsed);
      lastIndexUsed += 1;
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

  while (numPages < requiredPages) {

    // TODO:
    // this should really start from the beginning and try again
    if (index > bitmap.maxPages) {
      return std::nullopt;
    }

    if (getEntryFromBitmapIndex(baseIndex) != getEntryFromBitmapIndex(index)) {
      baseIndex = index;
      numPages = 0;
    }

    if (bitmap.isPageFree(index)) {
      numPages += 1;
    } else {
      baseIndex = index;
      numPages = 0;
    }

    index += 1;
  }

  lastIndexUsed = index;
  auto address = getAddressFromBitmapIndex(baseIndex);
  bitmap.setContiguousPagesAsUsed(baseIndex, index);
  return address;
}

auto BitmapAllocator::freePage(PhysicalAddress address) -> void {
  const auto index = getBitmapIndexFromAddress(address);

  if (!index) {
    Kernel::panic("Failed to free page that is out of range");
  }

  bitmap.setFree(index.value());
}

auto BitmapAllocator::freeContiguousPages(PhysicalAddress address, size_t numPages) -> void {
  const auto index = getBitmapIndexFromAddress(address);

  Kernel::println("index {}", index.value());
  if (!index) {
    Kernel::panic("Failed to free page, since it is out of range");
    return;
  }

  for (size_t i = index.value(); i < index.value() + numPages; i++) {
    bitmap.setFree(i);
  }
}

auto BitmapAllocator::getBitmapIndexFromEntry(limine_memmap_entry* entry) -> std::optional<size_t> {

  // calculate how many pages each entry has
  size_t numPages = 0;

  for (auto* current : memoryMap.usable) {
    if (current == entry) {
      break;
    }

    numPages += current->length/PAGE_SIZE;
  }

  return numPages;
}

// TODO: broken
auto BitmapAllocator::getEntryFromBitmapIndex(size_t index) -> std::optional<limine_memmap_entry*> {

  for (auto* entry : memoryMap.usable) {
    const auto pagesInEntry = entry->length / PAGE_SIZE;

    if (index > pagesInEntry) {
      index -= pagesInEntry;
      continue;
    }

    // we found the entry we're looking for
    return entry;
  }

  return std::nullopt;
}

// TODO: broken
auto BitmapAllocator::getAddressFromBitmapIndex(size_t index) -> std::optional<PhysicalAddress> {

  for (auto* entry : memoryMap.usable) {
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
  for (auto* current : memoryMap.usable) {
    if (address >= current->base && address <= current->base + current->length) {
      entry = current;
      break;
    }
  }

  if (!entry) {
    return std::nullopt;
  }

  // I kept getting overflow here, turns out i was doing entry->base - address.
  // compute how many pages before we get to address
  size_t numPages = (address - entry->base) / PAGE_SIZE;
  // number of pages before the entry;
  size_t basePages = getBitmapIndexFromEntry(entry).value();
  return numPages + basePages;
}


auto BitmapAllocator::getBitmapData(size_t i) -> u8 {
  return bitmap.data[i];
}

auto BitmapAllocator::printInfo() -> void {

  Kernel::println("Bitmap");

  for (size_t i = 0; i < 40; i++) {
    const auto data = getBitmapData(i);

    Kernel::print("{} -", i);
    for (u8 j = 0; j < 8; j++) {
      Kernel::print("{} ", (data >> j) & 1);
    }
    Kernel::println("");
  }

  Kernel::println("Reserved {}", bitmap.reservedPages);
  Kernel::println("Used {}", bitmap.usedPages - bitmap.reservedPages);
  Kernel::println("Capacity {}", bitmap.maxPages);

  Kernel::println("");
}
