#include <kernel/utils/assert.h>
#include <kernel/arch/platform.h>
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
  return {(u64) header, header->getSize()};
}


auto Header::setSize(size_t value) -> void {
  data |= value & ~0b111;
}

auto Header::getSize() const -> size_t {
  return data &~ 0b111;
}

auto Header::isUsed() const -> bool {
  return sl::bit::get(data, 0);
}

auto Header::setIsUsed(bool value) -> void {
  sl::bit::setMut(data, 0, value);
}


Block::Block(Address startAddress, size_t size) : address(startAddress), size(size) { }

auto Block::installHeaders() -> void {
  auto* leftEndHeader= (Header*) address;
  auto* rightEndHeader = (Header*) (address + sizeof(Header) + size);

  Kernel::assert(rightEndHeader != nullptr);
  Kernel::assert(leftEndHeader != nullptr);

  rightEndHeader->setSize(size);
  leftEndHeader->setSize(size);

  Kernel::assert(rightEndHeader->getSize() == size);
  Kernel::assert(leftEndHeader->getSize() == size);
}

auto Block::getPayloadSize() const -> size_t {
  return size;
}

auto Block::getBlockSize() const -> size_t {
  return size + 2 * sizeof(Header);
}

auto Block::isUsed() const -> bool {
  const auto* header = (Header*) address;
  return header->isUsed();
}

auto Block::setIsUsed(bool value) -> void {
  auto* leftEndHeader = (Header*) address;
  auto* rightEndHeader = (Header*) (address + sizeof(Header) + size);

  leftEndHeader->setIsUsed(value);
  rightEndHeader->setIsUsed(value);
}

auto Block::getPayload() -> void* {
  return (void*) (address + sizeof(Header));
}

auto Block::fromAddress(void* addr) -> Block {
  auto* header = (Header*) ((u64) addr - sizeof(Header));
  return {(u64) header,  header->getSize()};
}


auto HeapAllocator::initialize(Address start, size_t size, VMFlag flags) -> void {
  this->start = start;
  this->current = start;
  this->end = start + size;
  this->flags = flags;
  
  this->isInitialized = true;
  this->pagesAllocated = 1;

  auto page = (u64) PMM::allocatePage();
  VMM::map(start, page, flags);
}

auto HeapAllocator::alloc(size_t size) -> void* {
  size = sl::math::alignUp(size, 8);

  Kernel::assert(isInitialized, "Heap allocator not initialized");
  Kernel::assert(size < PAGE_SIZE, "Cannot allocate bigger than page size!");
  Kernel::assert(current + sizeof(Header) + size < end, "Cannot allocate memory when range is full");

  auto* currentNode = freeListHead;
  while (currentNode != nullptr) {
    auto block = currentNode->getBlock();

    if (block.getPayloadSize() == size and not block.isUsed()) {
      auto* previousNode = currentNode->prev;

      // we are at the head of the freelist
      if (previousNode == nullptr) {
        freeListHead  = currentNode->next;
        freeListCurrent = freeListHead;
      } else {
        previousNode->next = currentNode->next;
      }

      block.setIsUsed(true);
      return block.getPayload();
    }

    currentNode = currentNode->next;
  }

  if (current - start + size > pagesAllocated * PAGE_SIZE) {
    const auto page = (u64) PMM::allocatePage();
    const auto address = sl::math::alignUp(current, PAGE_SIZE);
    VMM::map(address, page, flags);

    pagesAllocated += 1;
  }


  // we can store this object to the stack since the constructor writes the
  // data to the usable memory
  auto block = Block(current, size);
  block.installHeaders();
  block.setIsUsed(true);
  current += block.getBlockSize();
  return block.getPayload();
}

auto HeapAllocator::free(void* addr) -> void {
  // TODO: perform coalescing
  Kernel::assert(isInitialized, "Heap allocator not initialized");
  Kernel::assert((u64) addr > start and (u64) addr < end, "Tried to free invalid address");

  auto block = Block::fromAddress(addr);
  Kernel::assert(block.isUsed(), "Tried to free an address that is not used");
  block.setIsUsed(false);

  auto* node = (FreeListNode*) block.getPayload();
  Kernel::assert(node != nullptr);

  node->next = nullptr;
  node->prev = freeListCurrent;

  if (!freeListHead) {
    freeListHead = node;
    freeListCurrent = freeListHead;
  } else {
    freeListCurrent->next = node;
    freeListCurrent = node;
  }
}
