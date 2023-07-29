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
  
  usable.startPointer = &m_memmapResponse->entries[usable.first];
  usable.endPointer = &m_memmapResponse->entries[usable.last + 1];

  reserved.startPointer = &m_memmapResponse->entries[reserved.first];
  reserved.endPointer = &m_memmapResponse->entries[reserved.last + 1];

  acpiReclaimable.startPointer = &m_memmapResponse->entries[acpiReclaimable.first];
  acpiReclaimable.endPointer = &m_memmapResponse->entries[acpiReclaimable.last + 1];

  acpiNvs.startPointer = &m_memmapResponse->entries[acpiNvs.first];
  acpiNvs.endPointer = &m_memmapResponse->entries[acpiNvs.last + 1];

  badMemory.startPointer = &m_memmapResponse->entries[badMemory.first];
  badMemory.endPointer = &m_memmapResponse->entries[badMemory.last + 1];

  bootloaderReclaimable.startPointer = &m_memmapResponse->entries[bootloaderReclaimable.first];
  bootloaderReclaimable.endPointer = &m_memmapResponse->entries[bootloaderReclaimable.last + 1];

  kernelAndModules.startPointer = &m_memmapResponse->entries[kernelAndModules.first];
  kernelAndModules.endPointer = &m_memmapResponse->entries[kernelAndModules.last + 1];

  framebuffer.startPointer = &m_memmapResponse->entries[framebuffer.first];
  framebuffer.endPointer = &m_memmapResponse->entries[framebuffer.last + 1];
}

auto MemoryMap::operator[](size_t index) -> limine_memmap_entry* {
  if (index >= this->entryCount) 
    return nullptr;
  return this->m_memmapResponse->entries[index];
}

