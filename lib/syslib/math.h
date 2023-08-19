#pragma once

#include <type_traits>
#include <lib/types.h>

namespace sl::math {

  template <typename T>
  concept Integral = std::is_integral_v<T>;

  constexpr auto ceilDiv(const Integral auto dividend, const Integral auto divisor) -> Integral auto {
    return ((dividend + divisor - 1) / divisor);
  }

  constexpr auto alignDown(const Integral auto value, const Integral auto align) -> Integral auto {
    return (value / align) * align;
  }

  constexpr auto alignUp(const Integral auto value, const Integral auto align) -> Integral auto {
    return ((value + align - 1) / align) * align;
  }


}
