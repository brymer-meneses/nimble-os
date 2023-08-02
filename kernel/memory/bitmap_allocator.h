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
        size_t reservedPages = 0;

      public:
        Bitmap(u8* data, size_t offset, size_t maxPages);
        Bitmap() {};

        auto setFree(size_t index) -> void;
        auto setUsed(size_t index) -> void;
        auto freeAll() -> void;
        auto isPageFree(size_t index) -> bool;
        auto setContiguousPagesAsUsed(size_t start, size_t end);
    };

  public:
    BitmapAllocator() {};
    auto initialize() -> void;

    [[nodiscard]] auto allocatePage() -> std::optional<PhysicalAddress>;
    [[nodiscard]] auto allocateContiguousPages(size_t pages) -> std::optional<PhysicalAddress>;
    auto freePage(PhysicalAddress address) -> void;
    auto freeContiguousPages(PhysicalAddress, size_t pages) -> void;
    auto getBitmapData(size_t index) -> u8;

    auto printInfo() -> void;

  private:
    auto getAddressFromBitmapIndex(size_t index) -> std::optional<PhysicalAddress>;
    auto getBitmapIndexFromAddress(PhysicalAddress address) -> std::optional<size_t>;
    auto getEntryFromBitmapIndex(size_t index) -> std::optional<limine_memmap_entry*>;
    auto getBitmapIndexFromEntry(limine_memmap_entry* entry) -> std::optional<size_t>;

  private:
    MemoryMap& memoryMap = MemoryMap::get();
    Bitmap bitmap{};
    size_t entryIndex = memoryMap.usable.first;
    size_t lastIndexUsed = 0;
};
