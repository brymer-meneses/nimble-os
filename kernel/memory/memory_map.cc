#include "memory_map.h"
#include "memory.h"
#include "pmm.h"

#include <algorithm>
#include <limits>

#include <kernel/utils/print.h>
#include <kernel/utils/panic.h>

#include <lib/math.h>

static volatile auto memmapRequest = limine_memmap_request {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

using Memory::PAGE_SIZE;

MemoryMap::MemoryMap() {

  memmapResponse = memmapRequest.response;

  if (!memmapResponse) {
    Kernel::panic("memmapResponse is null");
  }

  auto entries = memmapResponse->entries;
  entryCount = memmapResponse->entry_count;

  std::sort(entries, entries + entryCount,
            [](const auto* a, const auto* b) -> bool {
              return a->type < b->type;
            });

  initializeRange();
}

auto MemoryMap::operator[](size_t index) -> limine_memmap_entry* {
  if (index >= entryCount) 
    return nullptr;
  return memmapResponse->entries[index];
}

auto MemoryMap::initializeRange() -> void {
  auto entries = memmapResponse->entries;

  for (size_t i = 0; i < entryCount; i++) {
    auto* entry = entries[i];

    switch (entry->type) {
      case LIMINE_MEMMAP_USABLE:
        usable.last = std::max(usable.last, i);
        usable.first = std::min(usable.first, i);
        usable.count += 1;
        usablePages += Math::ceilDiv(entry->length, PAGE_SIZE);
        break;
      case LIMINE_MEMMAP_RESERVED:
        reserved.last = std::max(reserved.last, i);
        reserved.first = std::min(reserved.first, i);
        reserved.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        acpiReclaimable.last = std::max(acpiReclaimable.last, i);
        acpiReclaimable.first = std::min(acpiReclaimable.first, i);
        acpiReclaimable.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_NVS:
        acpiNvs.last = std::max(acpiNvs.last, i);
        acpiNvs.first = std::min(acpiNvs.first, i);
        acpiNvs.count += 1;
        break;
      case LIMINE_MEMMAP_BAD_MEMORY:
        badMemory.last = std::max(badMemory.last, i);
        badMemory.first = std::min(badMemory.first, i);
        badMemory.count += 1;
        break;
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        bootloaderReclaimable.last = std::max(bootloaderReclaimable.last, i);
        bootloaderReclaimable.first = std::min(bootloaderReclaimable.first, i);
        bootloaderReclaimable.count += 1;
        break;
      case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        kernelAndModules.last = std::max(kernelAndModules.last, i);
        kernelAndModules.first = std::min(kernelAndModules.first, i);
        kernelAndModules.count += 1;
        break;
      case LIMINE_MEMMAP_FRAMEBUFFER:
        framebuffer.last = std::max(framebuffer.last, i);
        framebuffer.first = std::min(framebuffer.first, i);
        framebuffer.count += 1;
        break;
    };
  }
  
  // we need to assign the end and start pointer mainly for iteration
  usable.startPointer = &memmapResponse->entries[usable.first];
  usable.endPointer = &memmapResponse->entries[usable.last + 1];

  reserved.startPointer = &memmapResponse->entries[reserved.first];
  reserved.endPointer = &memmapResponse->entries[reserved.last + 1];

  acpiReclaimable.startPointer = &memmapResponse->entries[acpiReclaimable.first];
  acpiReclaimable.endPointer = &memmapResponse->entries[acpiReclaimable.last + 1];

  acpiNvs.startPointer = &memmapResponse->entries[acpiNvs.first];
  acpiNvs.endPointer = &memmapResponse->entries[acpiNvs.last + 1];

  badMemory.startPointer = &memmapResponse->entries[badMemory.first];
  badMemory.endPointer = &memmapResponse->entries[badMemory.last + 1];

  bootloaderReclaimable.startPointer = &memmapResponse->entries[bootloaderReclaimable.first];
  bootloaderReclaimable.endPointer = &memmapResponse->entries[bootloaderReclaimable.last + 1];

  kernelAndModules.startPointer = &memmapResponse->entries[kernelAndModules.first];
  kernelAndModules.endPointer = &memmapResponse->entries[kernelAndModules.last + 1];

  framebuffer.startPointer = &memmapResponse->entries[framebuffer.first];
  framebuffer.endPointer = &memmapResponse->entries[framebuffer.last + 1];
}

