#pragma once
#include <lib/types.h>
#include <lib/bit.h>
#include "vmm.h"

class HeapAllocator {

public:
  using Address = u64;

  struct Header {
    u64 data;

    auto getSize() const -> size_t;
    auto setSize(size_t size) -> void;

    auto isUsed() const -> bool;
    auto setIsUsed(bool value) -> void;
  } __attribute__((packed));

  struct Block {
  private:
    Address start;
    size_t size;

  public:
    auto isUsed() const -> bool;
    auto setIsUsed(bool value) -> void;

    auto getPayloadSize() const -> size_t;
    auto getBlockSize() const -> size_t;
    
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
  } __attribute__((packed));


  FreeListNode* freeListHead = nullptr;
  FreeListNode* freeListCurrent = nullptr;
  size_t pagesAllocated = 0;

  Address start;
  Address current;
  Address end;
  VMFlag flags;

  bool isInitialized = false;

public:
  HeapAllocator() = default;

  auto initialize(Address start, size_t size, VMFlag flags) -> void;
  auto alloc(size_t size) -> void*;
  auto free(void* addr) -> void;
};

