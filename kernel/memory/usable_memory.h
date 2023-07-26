#pragma once

#include "pmm.h"
#include <limine.h>
#include <kernel/utils/assert.h>
#include <optional>
#include <lib/math.h>


class UsableMemory {

  public:
    UsableMemory(limine_memmap_response* memmap);
    UsableMemory() {};

    auto operator[](size_t index) const -> std::optional<limine_memmap_entry*>;
    auto printInfo() const -> void;

  public:
    u64 highestAddress = 0;
    u64 usablePages = 0;
    u64 usableEntries = 0;

  private:
    limine_memmap_response* mMemmap = nullptr;
    limine_memmap_entry** mUsableEntriesArray = nullptr;
};
