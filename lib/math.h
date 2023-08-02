#pragma once

#include <type_traits>
#include "types.h"

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
  requires std::is_unsigned_v<T>
  constexpr auto setBit(const T& value, const size_t index) -> T {
    return value | (static_cast<T>(1) << index);
  }

  template<typename T>
  requires std::is_unsigned_v<T>
  constexpr auto unsetBit(const T& value, const size_t index) -> T {
    return value | ~(static_cast<T>(1) << index);
  }

  template<typename T>
  requires std::is_unsigned_v<T>
  constexpr auto getBit(const T value, const size_t index) -> bool {
    return (value >> index) & 1;
  }

}
