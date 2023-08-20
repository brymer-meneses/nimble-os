#pragma once
#include <lib/types.h>
#include <lib/syslib/linkedList.h>
#include "vmm.h"

class HeapAllocator {

public:
  using Address = u64;

  struct Header {
    bool isUsed;
    u64 size;
  };

  struct Block {
  private:
    Address address;
    size_t size;

  public:
    auto isUsed() const -> bool;
    auto setIsUsed(bool value) -> void;

    auto getPayloadSize() const -> size_t;
    
    auto getPayload() -> void*;
    auto getNext() -> Block*;
    auto getPrev() -> Block*;

    auto installHeaders() -> void;

    static auto fromAddress(void* addr) -> Block;

    Block(Address start, size_t size);
  };

  struct FreeListNode {
    FreeListNode* next = nullptr;
    FreeListNode* prev = nullptr;

    auto getBlock() const -> Block;
  };

  FreeListNode* mFreeListHead = nullptr;
  FreeListNode* mFreeListEnd = nullptr;
  VMObject* mVMObject = nullptr;
  size_t mTotalAllocated = 0;
  VMM* mVMM;

public:
  HeapAllocator() = default;

  auto initialize(VMM* vmm) -> void;
  auto alloc(size_t size) -> void*;
  auto realloc(void* addr, size_t size) -> void*;
  auto free(void* addr) -> void;
};

