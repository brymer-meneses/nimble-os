#pragma once
#include "allocator.h"
#include <lib/libc.h>
#include <kernel/utils/assert.h>

namespace sl {

  template <typename Data, IAllocator Allocator>
  class Vector {

  private:
    Allocator& mAllocator;
    Data* mData = nullptr;
    size_t mCapacity;
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

    auto operator[] (size_t index) const -> Data {
      Kernel::assert(index <= mSize - 1);
      return mData[index];
    }

  };

}
