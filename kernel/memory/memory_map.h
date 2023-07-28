#pragma once

#include <limine.h>
#include <cstddef>
#include <optional>

#include "pmm.h"
#include <tuple>
#include <limits>

class MemoryMap {

  public:
    auto operator[](size_t index) -> limine_memmap_entry*;
    static auto get() -> MemoryMap& {
      static MemoryMap instance;
      return instance;
    }

  private:
    MemoryMap();

  public:
    struct Range {
      size_t start = std::numeric_limits<size_t>::max();
      size_t end = 0;
      size_t count = 0;
    };
    Range usable{};
    Range reserved{};
    Range acpiReclaimaible{};
    Range acpiNvs{};
    Range badMemory{};
    Range bootloaderReclaimable{};
    Range kernelAndModules{};
    Range framebuffer{};

    size_t entryCount;
    size_t usablePages;

  private:
    limine_memmap_response* m_memmapResponse = nullptr;
    limine_hhdm_response* m_hhdmResponse = nullptr;
};
