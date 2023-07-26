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

  template<typename T>
  requires std::is_arithmetic_v<T>
  constexpr auto max(T v1, T v2) -> T {
    return v1 > v2 ? v1 : v2;
  }

}
