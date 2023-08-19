#pragma once
#include <concepts>
#include <cstddef>

namespace sl {

  template <typename T>
  concept Allocator = requires(T a, size_t size, void* ptr) {
    { a.alloc(size) } -> std::same_as<void*>;
    { a.free(ptr) } -> std::same_as<void>;
  };

} // namespace sl
