#include "memory_map.h"
#include "memory.h"
#include "pmm.h"

#include <algorithm>
#include <limits>

#include <kernel/utils/print.h>
#include <kernel/utils/panic.h>
#include <kernel/arch/arch.h>
#include <kernel/boot/boot.h>

#include <lib/syslib/math.h>


using arch::PAGE_SIZE;

MemoryMap::MemoryMap() {

  memmapResponse = boot::memmapRequest.response;

  if (!memmapResponse) {
    kernel::panic("memmapResponse is null");
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
        usable.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        usable.count += 1;
        break;
      case LIMINE_MEMMAP_RESERVED:
        reserved.last = std::max(reserved.last, i);
        reserved.first = std::min(reserved.first, i);
        reserved.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        reserved.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        acpiReclaimable.last = std::max(acpiReclaimable.last, i);
        acpiReclaimable.first = std::min(acpiReclaimable.first, i);
        acpiReclaimable.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        acpiReclaimable.count += 1;
        break;
      case LIMINE_MEMMAP_ACPI_NVS:
        acpiNvs.last = std::max(acpiNvs.last, i);
        acpiNvs.first = std::min(acpiNvs.first, i);
        acpiNvs.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        acpiNvs.count += 1;
        break;
      case LIMINE_MEMMAP_BAD_MEMORY:
        badMemory.last = std::max(badMemory.last, i);
        badMemory.first = std::min(badMemory.first, i);
        badMemory.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        badMemory.count += 1;
        break;
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        bootloaderReclaimable.last = std::max(bootloaderReclaimable.last, i);
        bootloaderReclaimable.first = std::min(bootloaderReclaimable.first, i);
        bootloaderReclaimable.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        bootloaderReclaimable.count += 1;
        break;
      case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        kernelAndModules.last = std::max(kernelAndModules.last, i);
        kernelAndModules.first = std::min(kernelAndModules.first, i);
        kernelAndModules.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        kernelAndModules.count += 1;
        break;
      case LIMINE_MEMMAP_FRAMEBUFFER:
        framebuffer.last = std::max(framebuffer.last, i);
        framebuffer.first = std::min(framebuffer.first, i);
        framebuffer.pages += sl::math::ceilDiv(entry->length, PAGE_SIZE);
        framebuffer.count += 1;
        break;
    };
  }

  usable.entries = entries;
  reserved.entries = entries;
  acpiReclaimable.entries = entries;
  acpiNvs.entries = entries;
  badMemory.entries = entries;
  bootloaderReclaimable.entries = entries;
  kernelAndModules.entries = entries;
  framebuffer.entries = entries;
}

