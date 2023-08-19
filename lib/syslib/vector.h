#pragma once
#include "allocator.h"
#include <optional>
#include <lib/libc.h>
#include <kernel/utils/assert.h>
#include <iterator>

namespace sl {

  template <typename Data, Allocator Allocator>
  class Vector {

  private:
    Allocator& mAllocator;
    Data* mData = nullptr;
    size_t mCapacity = 0;
    size_t mSize = 0;

  public:

    Vector(Allocator& allocator, size_t capacity) : mAllocator(allocator), mCapacity(capacity) {
      mData = reinterpret_cast<Data*>(mAllocator.alloc(mCapacity * sizeof(Data)));
    }

    auto push(Data data) -> void {
      mSize += 1;

      if (mSize > mCapacity) {
        mCapacity += 1;
        
        Data* newData = reinterpret_cast<Data*>(mAllocator.alloc(mCapacity * sizeof(Data)));
        std::memcpy(newData, mData, mSize * sizeof(Data));
        mAllocator.free((void*) mData);
        mData = newData;
      }

      mData[mSize-1] = data;
    }

    auto size() const -> size_t {
      return mSize;
    }

    auto capacity() const -> size_t {
      return mCapacity;
    }

    auto free() -> void {
      mAllocator.free(mData);
    }

    auto operator[] (size_t index) const -> std::optional<Data> {
      if (index >= mSize) {
        return {};
      }

      return mData[index];
    }

  public:
    struct Iterator {
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      
      using value_type = Data;
      using pointer = Data*;
      using reference = Data&;

      pointer mPtr = nullptr;

      explicit Iterator(pointer ptr) : mPtr(ptr) {};

      // prefix increment
      auto operator++() -> Iterator {
        mPtr += 1;
        return *this;
      }

      // postfix increment
      auto operator++(int) -> Iterator {
        Iterator tmp = *this;
        mPtr += 1;
        return tmp;
      }

      auto operator*() const -> reference& {
        return *mPtr;
      }

      auto operator->() const -> pointer {
        return mPtr;
      }

      friend auto operator==(const Iterator& a, const Iterator& b) -> bool {
        return a.mPtr == b.mPtr;
      }

      friend auto operator!=(const Iterator& a, const Iterator& b) -> bool {
        return a.mPtr != b.mPtr;
      }
    };

    auto begin() -> Iterator {
      return Iterator(&mData[0]);
    }

    auto end() -> Iterator {
      return Iterator(&mData[mSize-1]);
    }

  };

}
