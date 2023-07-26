#include "usable_memory.h"
#include <kernel/utils/panic.h>

using PMM::PAGE_SIZE;

UsableMemory::UsableMemory(limine_memmap_response* memmap) : mMemmap(memmap) {
  KERNEL_ASSERT(memmap);

  // parse the memory map
  for (size_t i = 0; i < memmap->entry_count; i++) {
    auto* entry = memmap->entries[i];

    if (entry->type != LIMINE_MEMMAP_USABLE) 
      continue;

    usableEntries += 1;
  }

  auto usableEntriesSize = usableEntries * sizeof(uintptr_t);

  // find memory address for `mUsableEntriesArray`
  for (size_t i = 0; i < memmap->entry_count; i++) {
    auto* entry = memmap->entries[i];

    if (entry->type != LIMINE_MEMMAP_USABLE)
      continue;

    if (entry->length >= usableEntriesSize) {
      mUsableEntriesArray = (limine_memmap_entry**) entry->base;
      entry->base += usableEntriesSize;
      entry->length -= usableEntriesSize;
      break;
    }
  }

  if (!mUsableEntriesArray) {
    Kernel::panic("Cannot find memory for usable entries array");
  }

  // store the usable entries
  for (size_t i=0, j=0; i<memmap->entry_count; i++) {
    auto* entry = memmap->entries[i];

    if (entry->type != LIMINE_MEMMAP_USABLE)
      continue;

    usablePages += Math::ceilDiv(entry->length, PAGE_SIZE);
    highestAddress = Math::max(highestAddress, entry->base + entry->length);

    mUsableEntriesArray[j] = entry;
    j += 1;
  }
}

auto UsableMemory::operator[](size_t index) const -> std::optional<limine_memmap_entry*> {
  if (index >= usableEntries) 
    return std::nullopt;

  return mUsableEntriesArray[index];
}

auto UsableMemory::printInfo() const -> void {
  u64 highestAddress = 0;
  u64 usablePages = 0;
  u64 reservedPages = 0;

  for (size_t i=0; i<mMemmap->entry_count; i++) {

    limine_memmap_entry* entry = mMemmap->entries[i];

    Kernel::println("Memory Map Entry: base={hex} length={hex} type={hex}",
                    entry->base, entry->length, entry->type);

    switch (entry->type) {
      case LIMINE_MEMMAP_USABLE:
        usablePages += entry->length / PAGE_SIZE;
        highestAddress = Math::max(highestAddress, entry->length + entry->base);
        break;
      case LIMINE_MEMMAP_RESERVED:
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
      case LIMINE_MEMMAP_ACPI_NVS:
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
      case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        reservedPages += entry->length / PAGE_SIZE;
        break;
    }
  }

  Kernel::println("Usable Pages: {} mb", (usablePages * PAGE_SIZE ) / 1024 / 1024);
  Kernel::println("Reserved Pages: {} mb", (reservedPages * PAGE_SIZE) / 1024 / 1024);
};

