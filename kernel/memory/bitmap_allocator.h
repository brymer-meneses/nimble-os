#pragma once

#include <lib/types.h>
#include <lib/thirdparty/limine.h>
#include <cstddef>
#include <optional>
#include "pmm.h"
#include "usable_memory.h"

class BitmapAllocator {

  public:
    BitmapAllocator();

    auto allocatePage() -> std::optional<PhysicalAddress>;
    auto freePage(PhysicalAddress address) -> void;
    auto operator[](size_t index) -> u8;

  private:
    auto setFree(size_t index) -> void;
    auto setUsed(size_t index) -> void;
    auto freeAll() -> void;

    auto getUsableMemmapIndex() -> std::optional<size_t>;
    auto getMemmapIndex(size_t index) -> std::optional<limine_memmap_entry*>; 
    auto isBitFree(size_t index) -> bool;

  private:
    u8* mBitmapData = nullptr;
    size_t mBitmapSize = 0;
    size_t mLastIndex = 0;
    limine_memmap_response* mMemmap;
    limine_hhdm_response* mHhdm;
    // just a wrapper for `mMemmap->entries`
    UsableMemory mMemory;
};
