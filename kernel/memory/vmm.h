#pragma once

#include <lib/types.h>

struct VMFlag {
  bool userAccessible = false;
  bool writeable = false;
  bool executable = false;
};

struct VMObject {
  uintptr_t base;
  size_t totalPages;
  bool isUsed;
  VMObject* next;

  auto canFit(size_t length) const -> bool;
};

class VMM {

public:

  VMObject* mRoot = nullptr;
  VMObject* mCurrent = nullptr;
  uintptr_t mBaseAddress;
  uintptr_t mCurrentAddress;
  uintptr_t* mPageMap;
  VMFlag mFlags;

public:
  VMM() = default;

  auto initialize(uintptr_t* pageMap, uintptr_t baseAddress, VMFlag flags) -> void;
  auto alloc(size_t length) -> VMObject*;
  auto free(uintptr_t virtualAddress) -> bool;

  auto makeMain() -> void;

  struct HeapAllocator;
  friend HeapAllocator;
};
