#include "memory_map.h"
#include "pmm.h"

#include <algorithm>
#include <limits>

#include <kernel/utils/print.h>
#include <lib/math.h>

static volatile auto memmapRequest = limine_memmap_request{
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

static volatile auto hhdmRequest = limine_hhdm_request{
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

using PMM::PAGE_SIZE;

MemoryMap::MemoryMap() {

  m_memmapResponse = memmapRequest.response;
  // m_hhdmResponse = hhdmRequest.response;

  auto entries = m_memmapResponse->entries;
  entryCount = m_memmapResponse->entry_count;

  std::sort(entries, entries + entryCount,
            [](const auto* a, auto* b) -> bool {
              return a->type < b->type;
            });

  for (size_t i = 0; i < entryCount; i++) {
    auto* entry = entries[i];

    switch (entry->type) {
      case LIMINE_MEMMAP_USABLE:
        usable.end = std::max(usable.end, i);
        usable.start = std::min(usable.start, i);
        usable.count += 1;
        usablePages += Math::ceilDiv(entry->length, PAGE_SIZE);
        break;
      case LIMINE_MEMMAP_RESERVED:
        reserved.end = std::max(reserved.end, i);
        reserved.start = std::min(reserved.start, i);
        reserved.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        acpiReclaimaible.end = std::max(acpiReclaimaible.end, i);
        acpiReclaimaible.start = std::min(acpiReclaimaible.start, i);
        acpiReclaimaible.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_NVS:
        acpiNvs.end = std::max(acpiNvs.end, i);
        acpiNvs.start = std::min(acpiNvs.start, i);
        acpiNvs.count += 1;
        break;
      case LIMINE_MEMMAP_BAD_MEMORY:
        badMemory.end = std::max(badMemory.end, i);
        badMemory.start = std::min(badMemory.start, i);
        badMemory.count += 1;
        break;
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        bootloaderReclaimable.end = std::max(bootloaderReclaimable.end, i);
        bootloaderReclaimable.start = std::min(bootloaderReclaimable.start, i);
        bootloaderReclaimable.count += 1;
        break;
      case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        kernelAndModules.end = std::max(kernelAndModules.end, i);
        kernelAndModules.start = std::min(kernelAndModules.start, i);
        kernelAndModules.count += 1;
        break;
      case LIMINE_MEMMAP_FRAMEBUFFER:
        framebuffer.end = std::max(framebuffer.end, i);
        framebuffer.start = std::min(framebuffer.start, i);
        framebuffer.count += 1;
        break;
    };
  }
}

auto MemoryMap::operator[](size_t index) -> limine_memmap_entry* {
  if (index >= this->entryCount) 
    return nullptr;
  return this->m_memmapResponse->entries[index];
}

