#pragma once

#include <lib/types.h>
#include <lib/thirdparty/limine.h>
#include <cstddef>
#include <optional>
#include "pmm.h"
#include "memory_map.h"

class BitmapAllocator {

  public:
    BitmapAllocator();

    auto allocatePage() -> std::optional<PhysicalAddress>;
    auto freePage(PhysicalAddress address) -> void;
    auto operator[](size_t index) -> u8;
    auto allocatePages(size_t index) -> std::optional<PhysicalAddress>;

  private:
    auto setFree(size_t index) -> void;
    auto setUsed(size_t index) -> void;
    auto freeAll() -> void;

    auto getUsableMemmapIndex() -> std::optional<size_t>;
    auto getMemmapIndex(size_t index) -> std::optional<limine_memmap_entry*>; 
    auto isBitFree(size_t index) -> bool;

  private:
    size_t m_memoryIndex = 0; 
    u64 m_bitmapIndex = 0;
    u64 m_entryIndex = 0;
    u8* m_bitmapData = nullptr;
    size_t m_bitmapSize = 0;
    size_t m_lastIndex = 0;
    MemoryMap& memoryMap = MemoryMap::get();
};
