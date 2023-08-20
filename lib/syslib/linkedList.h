#pragma once

#include <lib/syslib/allocator.h>
#include <kernel/utils/print.h>
#include <iterator>
#include <optional>

namespace sl {

  template <typename ElementType, sl::Allocator Allocator>
  class LinkedList {

  private:
    struct Node {
      ElementType data;
      Node* next = nullptr;
      Node* prev = nullptr;
    };

    Node* mHead = nullptr;
    Node* mCurrent = nullptr;
    Allocator& mAllocator;

  private:
    auto constructNode(ElementType elem) -> Node* {
      auto* node = (Node*) mAllocator.alloc(sizeof(Node));
      node->data = elem;
      node->prev = nullptr;
      node->next = nullptr;
      return node;
    }

  public:
    LinkedList(Allocator& allocator) : mAllocator(allocator) {};

    auto remove(ElementType elem) -> bool {
      if (mHead == nullptr) {
        return false;
      }

      auto* node = mHead;
      while (node != nullptr) {

        if (node->data != elem) {
          node = node->next;
          continue;
        }

        auto* previousNode = node->prev;
        auto* nextNode = node->next;

        // we are at the beginning of the list
        if (previousNode == nullptr) {
          mHead = nextNode;
          mCurrent = nextNode;
          mAllocator.free(node);
          return true;
        }
        
        // we are at the last element of the list
        if (nextNode == nullptr) {
          mCurrent = previousNode;
          mAllocator.free(node);
          return true;
        }

        previousNode->next = nextNode;
        nextNode->prev = previousNode;
        node = nextNode;
      };

      return false;
    }

    auto append(ElementType elem) -> void {
      auto* node = constructNode(elem);
      if (mCurrent == nullptr) {
        mCurrent = node;
        mHead = node;
        return;
      }
      
      node->prev = mCurrent;
      mCurrent->next = node;
      mCurrent = node;
    }

    auto prepend(ElementType elem) -> void {
      auto* node = constructNode(elem);
      if (mCurrent == nullptr) {
        mCurrent = node;
        mHead = node;
        return;
      }

      node->next = mHead;
      mHead->prev = node;
      mHead = node;
    }

    auto free() -> void {
      auto* node = mHead;
      while (node != nullptr) {
        mAllocator.free(node);
        node = node->next;
      }
    };

    auto operator[] (size_t index) const -> std::optional<ElementType> {
      if (mHead == nullptr) {
        return {};
      }

      auto* node = mHead;
      for (size_t i=0; i<index; i++) {
        if (node == nullptr) {
          return {};
        }
        node = node->next;
      }

      return node->data;
    }

  public:
    class Iterator {
    private:
      Node* mNode;

    public:
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      
      using value_type = ElementType;
      using pointer = value_type*;
      using reference = value_type&;

      explicit Iterator(Node* node) : mNode(node) {}; 
      
      auto operator++() -> Iterator {
        mNode = mNode->next;
        return *this;
      }

      // postfix increment
      auto operator++(int) -> Iterator {
        auto* tmp = mNode;
        mNode = mNode->next;
        return Iterator(tmp);
      }

      auto operator*() const -> reference& {
        return mNode->data;
      }

      auto operator->() const -> pointer {
        return mNode->data;
      }

      friend auto operator==(const Iterator& a, const Iterator& b) -> bool {
        return a.mNode->data == b.mNode->data;
      }

      friend auto operator!=(const Iterator& a, const Iterator& b) -> bool {
        return a.mNode->data != b.mNode->data;
      }
    };

    auto begin() -> Iterator {
      return Iterator(mHead);
    }
    
    auto end() -> Iterator {
      return Iterator(mCurrent);
    }
  };

}
