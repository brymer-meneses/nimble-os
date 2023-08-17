#pragma once

#include <limine.h>
#include <cstddef>
#include <optional>

#include "pmm.h"
#include <tuple>
#include <limits>
#include <iterator>

class MemoryMap {

  public:
    auto operator[](size_t index) -> limine_memmap_entry*;
    static auto get() -> MemoryMap& {
      static MemoryMap instance;
      return instance;
    }

  private:
    MemoryMap();
    auto initializeRange() -> void;

  public:
    struct Iterator;
    class Range {
      using Iterator = MemoryMap::Iterator;

      public:
        size_t first = std::numeric_limits<size_t>::max();
        size_t last = 0;
        size_t count = 0;
        size_t pages = 0;

      private:
        limine_memmap_entry** entries = nullptr;
      
      public:
        auto begin() -> Iterator {
          return &entries[first];
        }

        auto end() -> Iterator {
          return &entries[last + 1];
        }

      friend class MemoryMap;
    };

    Range usable;
    Range reserved;
    Range acpiReclaimable;
    Range acpiNvs;
    Range badMemory;
    Range bootloaderReclaimable;
    Range kernelAndModules;
    Range framebuffer;

    size_t entryCount;

  private:
    limine_memmap_response* memmapResponse = nullptr;

  public:

    auto begin() -> Iterator {
      return &memmapResponse->entries[0];
    }

    auto end() -> Iterator {
      return &memmapResponse->entries[entryCount];
    }
    
    struct Iterator {
      // this is used 
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      
      using value_type = limine_memmap_entry*;
      using pointer = value_type*;
      using reference = value_type&;

      pointer ptr;

      Iterator(pointer ptr) : ptr(ptr) {};

      // prefix increment
      auto operator++() -> Iterator {
        ptr += 1;
        return *this;
      }

      // postfix increment
      auto operator++(int) -> Iterator {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
      }

      auto operator*() const -> reference& {
        return *ptr;
      }

      auto operator->() const -> pointer {
        return ptr;
      }

      friend auto operator==(const Iterator& a, const Iterator& b) -> bool {
        return a.ptr == b.ptr;
      }

      friend auto operator!=(const Iterator& a, const Iterator& b) -> bool {
        return a.ptr != b.ptr;
      }
    };

};
