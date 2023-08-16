#include <kernel/utils/assert.h>
#include <lib/math.h>
#include "heap_allocator.h"
#include "pmm.h"

using Memory::PAGE_SIZE;

using Block = HeapAllocator::Block;
using Header = HeapAllocator::Header;
using FreeListNode = HeapAllocator::FreeListNode;

auto FreeListNode::getBlock() const -> Block {
  const auto* header = (Header*) ((u8*) this - sizeof(FreeListNode) - sizeof(Header));
  return {(u64) header, header->getSize()};
}


auto Header::setSize(size_t value) -> void {
  data |= value & ~0b111;
}

auto Header::getSize() const -> size_t {
  return data &~ 0b111;
}

auto Header::isUsed() const -> bool {
  return Bit::get(data, 0);
}

auto Header::setIsUsed(bool value) -> void {
  Bit::setMut(data, 0, value);
}


Block::Block(Address startAddress, size_t size) : start(startAddress), size(size) { }

auto Block::installHeaders() -> void {
  auto* leftEndHeader= (Header*) start;
  auto* rightEndHeader = (Header*) (start + sizeof(Header) + size);

  Kernel::assert(rightEndHeader != nullptr);
  Kernel::assert(leftEndHeader != nullptr);

  rightEndHeader->setSize(size);
  leftEndHeader->setSize(size);

  Kernel::assert(rightEndHeader->getSize() == size);
  Kernel::assert(leftEndHeader->getSize() == size);
}

auto Block::getPayloadSize() const -> size_t {
  const auto* header = (Header*) start;
  return header->getSize();
}

auto Block::getBlockSize() const -> size_t {
  const auto* header = (Header*) start;
  return header->getSize() + 2 * sizeof(Header);
}

auto Block::isUsed() const -> bool {
  const auto* header = (Header*) start;
  return header->isUsed();
}

auto Block::setIsUsed(bool value) -> void {
  auto* leftEndHeader = (Header*) start;
  auto* rightEndHeader = (Header*) ((u8*) start + sizeof(Header) + leftEndHeader->getSize());

  leftEndHeader->setIsUsed(value);
  rightEndHeader->setIsUsed(value);
}

auto Block::getPayload() -> void* {
  return (u8*) start + sizeof(Header);
}

auto Block::fromAddress(void* addr) -> Block {
  auto* header = (Header*) ((u8*) addr - sizeof(Header));
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
  Kernel::assert(isInitialized);

  size = Math::alignUp(size, 8);
  Kernel::assert(current + sizeof(Header) + size < end, "Cannot allocate memory when range is full");

  auto* node = freeListHead;

  while (node != nullptr) {
    auto block = node->getBlock();

    if (block.getPayloadSize() == size and not block.isUsed()) {
      return block.getPayload();
    }

    node = node->next;
  }

  if (current > current + pagesAllocated * PAGE_SIZE) {
    const auto page = (u64) PMM::allocatePage();
    const auto address = Math::alignDown(current, PAGE_SIZE);
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
  Kernel::assert(isInitialized);
  Kernel::assert((u64) addr > start and (u64) addr < end, "Tried to free invalid address");

  auto block = Block::fromAddress(addr);

  Kernel::assert(block.isUsed(), "Tried to free an address that is not used");

  auto* node = (FreeListNode*) block.getPayload();
  Kernel::assert(node != nullptr);

  node->next = nullptr;
  node->prev = nullptr;

  if (!freeListHead) {
    freeListHead = node;
    freeListCurrent = freeListHead;
    return;
  }

  freeListCurrent->next = node;
  freeListCurrent = node;
  block.setIsUsed(false);
}
