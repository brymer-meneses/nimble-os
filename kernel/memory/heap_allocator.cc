#include <kernel/utils/assert.h>
#include <kernel/arch/arch.h>
#include <kernel/utils/logger.h>
#include <algorithm>
#include <lib/syslib/bit.h>
#include <lib/syslib/math.h>
#include "heap_allocator.h"
#include "pmm.h"

using arch::PAGE_SIZE;

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

  kernel::assert(rightEndHeader != nullptr);
  kernel::assert(leftEndHeader != nullptr);

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
  kernel::assert(header1->isUsed == header2->isUsed);
  return header1->isUsed && header2->isUsed;
}

auto Block::setIsUsed(bool value) -> void {
  auto* leftEndHeader = (Header*) address;
  auto* rightEndHeader = (Header*) (address + sizeof(Header) + size);

  leftEndHeader->isUsed = value;
  rightEndHeader->isUsed = value;

  kernel::assert(leftEndHeader->isUsed == rightEndHeader->isUsed);
  kernel::assert(leftEndHeader->size == rightEndHeader->size);
}

auto Block::getPayload() -> void* {
  return (void*) (address + sizeof(Header));
}

auto Block::fromAddress(void* addr) -> Block {
  auto* leftEndHeader = (Header*) ((u64) addr - sizeof(Header));
  auto* rightEndHeader = (Header*) ((u64) addr + leftEndHeader->size);

  kernel::assert(leftEndHeader->size == rightEndHeader->size);
  kernel::assert(leftEndHeader->isUsed == rightEndHeader->isUsed);

  return {(u64) leftEndHeader,  leftEndHeader->size};
}

auto HeapAllocator::initialize(VMM* vmm) -> void {
  mVMM = vmm;
  mVMObject = vmm->alloc(1);
  mTotalAllocated = 0;
}

auto HeapAllocator::alloc(size_t payloadSize) -> void* {

  // NOTE: Multiplying by 2 seems to fix some obscure bugs
  // TODO: figure out why this fixes it
  //
  // ensure that we can fit a freeListNode in the payload
  payloadSize = std::max(payloadSize, 2 * sizeof(FreeListNode));

  auto* node = mFreeListHead;
  while (node != nullptr) {
    auto block = node->getBlock();

    // this is an explicit free list so block must not be used
    kernel::assert(not block.isUsed());

    if (block.getPayloadSize() < payloadSize) {
      node = node->next;
      continue;
    }

    // once we find a block with the appropriate size
    // we could then remove it from the freelist
    auto* prevNode = node->prev;
    auto* nextNode = node->next;

    if (prevNode == nullptr) {
      mFreeListHead = nextNode;

      // This caused some really nasty bugs when I didn't take account for this
      if (mFreeListHead != nullptr) {
        // set the previous node to nullptr
        mFreeListHead->prev = nullptr;
      } else {
        // if the next node is a nullptr
        // then we can assume that this is the end of the list
        mFreeListEnd = nextNode;
      }

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
    const auto pages = sl::math::ceilDiv(blockSize, PAGE_SIZE);
    mVMObject = mVMM->alloc(pages);
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
  kernel::assert(block.isUsed(), "Tried to free an address that is not used");

  block.setIsUsed(false);

  auto* freeListNode = (FreeListNode*) block.getPayload();
  kernel::assert(freeListNode != nullptr);

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
