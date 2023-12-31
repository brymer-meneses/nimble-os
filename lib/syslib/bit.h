#pragma once

#include <type_traits>
#include <cstddef>

namespace sl::bit {

  template<typename T>
  concept Unsigned = std::is_unsigned_v<T>;

  // Mutable Functions
  template<Unsigned T>
  constexpr auto setMut(T& value, const size_t index) -> void {
    value |= (static_cast<T>(1) << index);
  }

  template<Unsigned T>
  constexpr auto unsetMut(T& value, const size_t index) -> void {
    value &= ~(static_cast<T>(1) << index);
  }

  template<Unsigned T>
  constexpr auto setMut(T& value, const size_t index, bool shouldSet) -> void {
    if (shouldSet) {
      setMut(value, index);
    } else {
      unsetMut(value, index);
    }
  }

  // Immutable Functions
  template<Unsigned T>
  [[nodiscard]] constexpr auto set(const T& value, const size_t index) -> T {
    return value | (static_cast<T>(1) << index);
  }

  template<Unsigned T>
  [[nodiscard]] constexpr auto set(const T& value, const size_t index, bool shouldSet) -> T {
    if (shouldSet) {
      return set(value, index);
    } else {
      return unset(value, index);
    }
  }

  template<Unsigned T>
  [[nodiscard]] constexpr auto unset(const T& value, const size_t index) -> T {
    return value & ~(static_cast<T>(1) << index);
  }

  template<Unsigned T>
  [[nodiscard]] constexpr auto get(const T& value, const size_t index) -> bool {
    return (value >> index) & static_cast<T>(1);
  }

  template<Unsigned T>
  [[nodiscard]] constexpr auto unsetRight(const T value, const size_t bits) -> T {
    const T maxBitValue = (static_cast<T>(1) << bits) - static_cast<T>(1);
    return ~(value & maxBitValue);
  }

  template<Unsigned T>
  [[nodiscard]] constexpr auto unsetLeft(const T value, const size_t bits) -> T {
    return ~(value << (sizeof(T) * 8 - bits));
  }

  template<Unsigned T>
  constexpr auto unsetRightMut(T& value, const size_t bits) -> void {
    const T maxBitValue = (static_cast<T>(1) << bits) - static_cast<T>(1);
    value = ~(value & maxBitValue);
  }

  template<Unsigned T>
  constexpr auto unsetLeftMut(T& value, const size_t bits) -> void {
    const T maxBitValue = (static_cast<T>(1) << bits) - static_cast<T>(1);
    value = ~(value & (maxBitValue << (sizeof(T) * 8 - bits)));
  }

  /// mask bits starting from the left to the right
  template<Unsigned T>
  [[nodiscard]] constexpr auto mask(const T& value, const size_t left, const size_t right) -> T {
    auto masked = unsetLeft(value, left);
    masked = unsetRight(masked, right);
    return masked;
  }

}
