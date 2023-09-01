#include <limine.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>
#include <kernel/utils/assert.h>
#include <lib/syslib/math.h>
#include <lib/syslib/bit.h>

#include "vmm.h"
#include "pmm.h"
#include "memory.h"

#include <kernel/arch/arch.h>

using arch::PAGE_SIZE;

auto VMObject::canFit(size_t length) const -> bool {
  const auto totalCapacity = totalPages * PAGE_SIZE - sizeof(VMObject);
  return length <= totalCapacity;
}

auto VMM::initialize(uintptr_t* pageMap, uintptr_t baseAddress, VMFlag flags) -> void {
  mFlags = flags;
  mBaseAddress = baseAddress;
  mCurrentAddress = mBaseAddress;
  mPageMap = pageMap;
}

auto VMM::alloc(size_t pages) -> VMObject* {

  for (size_t i = 0; i < pages; i++) {
    auto page = (uintptr_t) PMM::allocatePage();
    const auto address = mCurrentAddress + i * PAGE_SIZE;
    arch::paging::map(mPageMap, address, page, mFlags);
  }

  auto* node = mRoot;
  while (node != nullptr) {
    if (not node->isUsed and node->totalPages >= pages) {
      node->isUsed = true;
      return node;
    };
    node = node->next;
  }

  auto* object = (VMObject*) mCurrentAddress;

  object->next = nullptr;
  object->isUsed = true;
  object->totalPages = pages;
  object->base = mCurrentAddress + sizeof(VMObject);

  if (mRoot == nullptr) {
    mRoot = object;
    mCurrent = object;
  } else {
    mCurrent->next = object;
    mCurrent = object;
  }

  mCurrentAddress += pages * PAGE_SIZE;
  return object;
}

auto VMM::free(uintptr_t address) -> bool {
  auto* node = mRoot;
  while (node != nullptr) {
    if (node->base == address) {
      node->isUsed = false;
      return true;
    }
    node = node->next;
  }
  return false;
}

