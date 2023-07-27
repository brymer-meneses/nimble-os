#include "bitmap_allocator.h"
#include "pmm.h"

#include <kernel/utils/panic.h>
#include <kernel/utils/assert.h>
#include <lib/math.h>

#include "memory_map.h"

using PMM::PAGE_SIZE;


BitmapAllocator::BitmapAllocator() {

  // the bitmap is a u8 array, each `u8` corresponds to 8 pages
  m_bitmapSize = Math::ceilDiv(memoryMap.usable.count, 8);

  // find a place for the bitmap
  for (size_t i=0; i<memoryMap.usable.count; i++) {

    auto* entry = memoryMap[i];
    if (entry->length >= m_bitmapSize) {
      m_bitmapData = (u8*) entry->base;
      entry->base += m_bitmapSize;
      entry->length -= m_bitmapSize;
      break;
    }
  }

  if (!m_bitmapData) {
    Kernel::panic("Cannot find a place for `mBitmapData`");
  }
  
  freeAll();
}

auto BitmapAllocator::setFree(size_t i) -> void {
  auto row = i / 8;
  auto col = i % 8;
  m_bitmapData[row] = m_bitmapData[row] & ~(1 << col);
}

auto BitmapAllocator::setUsed(size_t i) -> void {
  auto row = i / 8;
  auto col = i % 8;
  m_bitmapData[row] = m_bitmapData[row] | (1 << col);
}

auto BitmapAllocator::freeAll() -> void {
  for (size_t i=0; i<m_bitmapSize; i++) {
    m_bitmapData[i] = 0;
  }
}

auto BitmapAllocator::isBitFree(size_t i) -> bool {
  auto row = i / 8;
  auto col = i % 8;
  return (m_bitmapData[row] & (1 << col)) == 0;
}

auto BitmapAllocator::allocatePage() -> std::optional<PhysicalAddress> {

  while (m_bitmapIndex < memoryMap.usable.count) {

    auto* entry = memoryMap[m_memoryIndex];
    while (m_entryIndex * PAGE_SIZE >= entry->length) {
      m_memoryIndex += 1;
      if (m_memoryIndex >= memoryMap.usable.count) {
        Kernel::panic("Memory full");
        return std::nullopt;
      }
      m_entryIndex = 0;
      entry = memoryMap[m_memoryIndex];
    }

    if (isBitFree(m_bitmapIndex)) {
      auto address = entry->base + m_entryIndex * PAGE_SIZE;

      setUsed(m_bitmapIndex);
      m_bitmapIndex += 1;
      m_entryIndex += 1;
      return address;
    }

    m_bitmapIndex += 1;
  }

  return std::nullopt;
}

auto BitmapAllocator::allocatePages(size_t index) -> std::optional<PhysicalAddress> {

}

auto BitmapAllocator::freePage(PhysicalAddress address) -> void {
  limine_memmap_entry* entry = nullptr;

  for (size_t i = 0; i < memoryMap.usable.count; i++) {
    entry = memoryMap[i];
    if (address >= entry->base && address < entry->base + entry->length) {
      break;
    }
  }

  setFree((address - entry->base) / PAGE_SIZE);
}

auto BitmapAllocator::operator[](size_t i) -> u8 {
  return m_bitmapData[i];
}
