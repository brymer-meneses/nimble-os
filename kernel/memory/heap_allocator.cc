#include <kernel/utils/assert.h>
#include <kernel/arch/platform.h>
#include <algorithm>
#include <lib/syslib/bit.h>
#include <lib/syslib/math.h>
#include "heap_allocator.h"
#include "pmm.h"

using Arch::PAGE_SIZE;

using Block = HeapAllocator::Block;
using Header = HeapAllocator::Header;
using FreeListNode = HeapAllocator::FreeListNode;

auto FreeListNode::getBlock() const -> Block {
  const auto* header = (Header*) ((u64) this - sizeof(Header));
  return {(u64) header, header->size};
}

Block::Block(Address startAddress, size_t size) : address(startAddress), size(size) { }

auto Block::installHeaders() -> void {
  auto* leftEndHeader= (Header*) address;
  auto* rightEndHeader = (Header*) (address + sizeof(Header) + size);

  Kernel::assert(rightEndHeader != nullptr);
  Kernel::assert(leftEndHeader != nullptr);

  leftEndHeader->isUsed = false;
  leftEndHeader->size = size;
  rightEndHeader->isUsed = false;
  rightEndHeader->size = size;
}

auto Block::getPayloadSize() const -> size_t {
  return size;
}

auto Block::isUsed() const -> bool {
  const auto* header1 = (Header*) address;
  auto* header2 = (Header*) (address + sizeof(Header) + size);
  Kernel::assert(header1->isUsed == header2->isUsed);
  return header1->isUsed && header2->isUsed;
}

auto Block::setIsUsed(bool value) -> void {
  auto* leftEndHeader = (Header*) address;
  auto* rightEndHeader = (Header*) (address + sizeof(Header) + size);

  leftEndHeader->isUsed = value;
  rightEndHeader->isUsed = value;
}

auto Block::getPayload() -> void* {
  return (void*) (address + sizeof(Header));
}

auto Block::fromAddress(void* addr) -> Block {
  auto* header = (Header*) ((u64) addr - sizeof(Header));
  return {(u64) header,  header->size};
}

auto HeapAllocator::initialize(VMM* vmm) -> void {
  mVMM = vmm;
  mVMObject = vmm->alloc(1);
  mTotalAllocated = 0;
}

auto HeapAllocator::alloc(size_t payloadSize) -> void* {

  payloadSize = std::max(payloadSize, sizeof(FreeListNode));

  auto* node = mFreeListHead;
  while (node != nullptr) {
    auto block = node->getBlock();

    // this is an explicit free list so block must not be used
    Kernel::assert(not block.isUsed());

    if (block.getPayloadSize() != payloadSize) {
      node = node->next;
      continue;
    }

    // once we find a block with the appropriate size
    // we could then remove it from the freelist
    auto* prevNode = node->prev;
    auto* nextNode = node->next;

    if (prevNode == nullptr) {
      mFreeListHead = node->next;
      mFreeListEnd = node->next;
    } else if (nextNode == nullptr) {
      mFreeListEnd = prevNode;
      mFreeListEnd->next = nullptr;
    } else {
      prevNode->next = nextNode;
      nextNode->prev = prevNode;
    }

    block.setIsUsed(true);
    return block.getPayload();
  }

  const auto blockSize = payloadSize + 2 * sizeof(Header);

  if (not mVMObject->canFit(mTotalAllocated + blockSize)) {
    mVMObject = mVMM->alloc(sl::math::ceilDiv(blockSize, PAGE_SIZE));
    mTotalAllocated = 0;
  }

  auto block = Block(mVMObject->base + mTotalAllocated, payloadSize);
  block.installHeaders();
  block.setIsUsed(true);
  mTotalAllocated += blockSize;
  return block.getPayload();
}

auto HeapAllocator::free(void* addr) -> void {

  auto block = Block::fromAddress(addr);
  Kernel::assert(block.isUsed(), "Tried to free an address that is not used");

  block.setIsUsed(false);

  auto* freeListNode = (FreeListNode*) block.getPayload();
  Kernel::assert(freeListNode != nullptr);

  freeListNode->next = nullptr;
  freeListNode->prev = nullptr;

  if (mFreeListHead == nullptr) {
    mFreeListHead = freeListNode;
    mFreeListEnd = freeListNode;
  } else {
    freeListNode->prev = mFreeListEnd;
    mFreeListEnd->next = freeListNode;
    mFreeListEnd = freeListNode;
  }
}
