#pragma once

#include <type_traits>

namespace Math {

  template <typename T, typename U>
  requires std::is_integral_v<T> && std::is_integral_v<U>
  constexpr auto ceilDiv(T dividend, U divisor) -> std::common_type_t<T, U> {
    return ((dividend + divisor - 1) / divisor);
  }

  template<typename T>
  requires std::is_integral_v<T>
  constexpr auto alignDown(T value, T align) -> T {
    return (value / align) * align;
  }
}
