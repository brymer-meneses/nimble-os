#pragma once
#include <type_traits>
#include <stddef.h>
#include "string_helpers.h"

namespace Format {

// This will be useful if we want to print custom structs
struct FormatArgument {
  virtual constexpr auto toString() const -> const char* = 0;
};


template<typename T>
[[nodiscard]] 
static constexpr auto appendArgument(char* buffer, size_t bpos, T value) -> int {
  if constexpr (std::is_same_v<const char*, T>) {
    return StringHelpers::appendToBuffer(buffer, bpos, value);
  }

  if constexpr (std::is_integral_v<T>) {
    char temp_buffer[64]; // should be sufficient
    StringHelpers::integralToString(value, 10, temp_buffer); // Use base 10 as default
    return StringHelpers::appendToBuffer(buffer, bpos, temp_buffer);
  }

  if constexpr (std::is_base_of_v<FormatArgument, T>) {
    const char* result = value.toString();
    return StringHelpers::appendToBuffer(buffer, bpos, result);
  }

  return 0;
}

static constexpr void formatImpl(char* buffer, const char* string, size_t bpos, size_t spos) {  

  while (string[spos] != '\0') {
    buffer[bpos] = string[spos];

    bpos += 1;
    spos += 1;
  }

  // terminate the buffer with a null character
  buffer[bpos] = '\0';
  return;
}

template<typename Arg, typename ...Args>
static constexpr void formatImpl(char* buffer, const char* string, size_t bpos, size_t spos, Arg arg, Args ...args) {

  while (string[spos] != '\0') {
    if (string[spos] == '{' && string[spos + 1] == '}') {
      spos += 2;
      bpos += appendArgument(buffer, bpos, arg);
      formatImpl(buffer, string, bpos, spos, args...);
      return;
    }

    buffer[bpos] = string[spos];
    bpos += 1;
    spos += 1;
  }

  // terminate the buffer with a null character
  buffer[bpos] = '\0';
  return;
}


template<typename Arg, typename ...Args>
constexpr void format(char* buffer, const char* string, Arg arg, Args ...args) {
  formatImpl(buffer, string, 0, 0, arg, args...);
}

}
