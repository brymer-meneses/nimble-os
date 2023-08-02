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

  public:
    struct Iterator;
    class Range {
      using Iterator = MemoryMap::Iterator;

      public:
        size_t first = std::numeric_limits<size_t>::max();
        size_t last = 0;
        size_t count = 0;

      private:
        limine_memmap_entry** startPointer = nullptr;
        limine_memmap_entry** endPointer = nullptr;
      
      public:
        auto begin() -> Iterator {
          return Iterator(startPointer);
        }

        auto end() -> Iterator {
          return Iterator(endPointer);
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
    size_t usablePages;

  private:
    limine_memmap_response* memmapResponse = nullptr;

  public:
    
    struct Iterator {
      // this is used 
      using IteratorCategory = std::forward_iterator_tag;
      using DifferenceType = std::ptrdiff_t;
      
      using ValueType = limine_memmap_entry*;
      using Pointer = ValueType*;
      using Reference = ValueType&;

      Pointer ptr;

      Iterator(Pointer ptr) : ptr(ptr) {};

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

      auto operator*() const -> Reference& {
        return *ptr;
      }

      auto operator->() const -> Pointer {
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
