#pragma once

#include <cstddef>
#include <cstdint>

namespace Heap {

  auto malloc(size_t size) -> void*;
  auto free(uintptr_t* address) -> void;

}
