#pragma once

#include <lib/types.h>
#include <lib/thirdparty/limine.h>
#include <cstddef>
#include <optional>
#include "pmm.h"
#include "memory_map.h"

class BitmapAllocator {

  public:
    class Bitmap {
      public:
        size_t usedPages = 0;
        u8* data = nullptr;
        size_t maxPages = 0;

      public:
        Bitmap(u8* data, size_t maxPages);
        Bitmap() {};

        auto setFree(size_t index) -> void;
        auto setUsed(size_t index) -> void;
        auto freeAll() -> void;
        auto isPageFree(size_t index) -> bool;
        auto setContiguousPagesAsUsed(size_t start, size_t end);
    };

  public:
    BitmapAllocator();

    auto allocatePage() -> std::optional<PhysicalAddress>;
    auto freePage(PhysicalAddress address) -> void;
    auto getBitmapData(size_t index) -> u8;
    auto allocateContiguousPages(size_t pages) -> std::optional<PhysicalAddress>;

  private:
    auto getAddressFromBitmapIndex(size_t index) -> std::optional<PhysicalAddress>;
    auto getBitmapIndexFromAddress(PhysicalAddress address) -> std::optional<size_t>;

  private:
    MemoryMap& memoryMap = MemoryMap::get();
    Bitmap bitmap{};
    size_t entryIndex = memoryMap.usable.start;
    size_t lastIndexUsed = 0;
};
