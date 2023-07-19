#pragma once
#include <stddef.h>
#include <type_traits>
#include <lib/libc.h>
#include <kernel/utils/string.h>

namespace {

enum class FormatSpecifier {
  Hexadecimal,
  Octal,
  Binary,
  Inferred,
};

template <typename T, FormatSpecifier specifier = FormatSpecifier::Inferred>
constexpr auto appendValue(char *buffer, size_t bpos, T value) -> int {

  if constexpr (std::is_integral_v<T>) {
    if constexpr (specifier == FormatSpecifier::Hexadecimal) {
      bpos += String::copy(buffer, bpos, "0x", 0, 3);

      char temp_buffer[256];
      int length = String::fromIntegral(value, temp_buffer, 16);
      bpos += String::copy(buffer, bpos, temp_buffer, 0, length+1);

      return bpos;
    }

    if constexpr (specifier == FormatSpecifier::Octal) {
      bpos += String::copy(buffer, bpos, "0o", 0, 3);

      char temp_buffer[256];
      int length = String::fromIntegral(value, temp_buffer, 8);
      bpos += String::copy(buffer, bpos, temp_buffer, 0, length+1);

      return bpos;
    }

    if constexpr (specifier == FormatSpecifier::Binary) {
      bpos += String::copy(buffer, bpos, "0b", 0, 3);
      char temp_buffer[256];
      int length = String::fromIntegral(value, temp_buffer, 2);
      bpos += String::copy(buffer, bpos, temp_buffer, 0, length+1);

      return bpos;
    }
  }


  if constexpr (specifier == FormatSpecifier::Inferred) {
    if constexpr (std::is_same_v<const char *, T>) {
      int i = 0;
      while (value[i] != '\0') {
        buffer[bpos] = value[i];
        bpos += 1;
        i++;
      }
      return i;
    }

    if constexpr (std::is_integral_v<T>) {
      char temp_buffer[256];
      int length = String::fromIntegral(value, temp_buffer, 10); // Use base 10 as default

      // Copy the characters to the buffer
      for (int i = 0; i < length; i++) {
        buffer[bpos] = temp_buffer[i];
        bpos += 1;
      }

      return length;
    }
  }
  return 0;
}

constexpr void formatImpl(char *buffer, const char *string, size_t bpos,
                           size_t spos) {

  while (string[spos] != '\0') {
    buffer[bpos] = string[spos];

    bpos += 1;
    spos += 1;
  }

  // terminate the buffer with a null character
  buffer[bpos] = '\0';
  return;
}

template <typename Arg, typename... Args>
constexpr void formatImpl(char *buffer, const char *string, size_t bpos,
                           size_t spos, Arg arg, Args... args) {

  while (string[spos] != '\0') {

    if (string[spos] != '{') {
      buffer[bpos] = string[spos];
      bpos += 1;
      spos += 1;
      continue;
    };

    int specifier_length = 0;
    int specifier_start = spos;
    while (string[spos] != '}' && string[spos] != '\0') {
      specifier_length++;
      spos += 1;
    }

    if (string[spos] == '\0') {
      String::copy(buffer, bpos, string, spos, specifier_length);
      return;
    }

    if (string[spos] == '}') {
      char specifier[16];
      String::copy(specifier, 0, string, specifier_start+1, specifier_start + specifier_length);
      spos += 1;

      if (specifier_length == 1) {
        bpos += appendValue<Arg, FormatSpecifier::Inferred>(buffer, bpos, arg);
      } else if (std::strcmp(specifier, "hex") == 0) {
        bpos += appendValue<Arg, FormatSpecifier::Hexadecimal>(buffer, bpos, arg);
      } else if (std::strcmp(specifier, "oct") == 0) {
        bpos += appendValue<Arg, FormatSpecifier::Octal>(buffer, bpos, arg);
      } else if (std::strcmp(specifier, "bin") == 0) {
        bpos += appendValue<Arg, FormatSpecifier::Binary>(buffer, bpos, arg);
      }
    }

    formatImpl(buffer, string, bpos, spos, args...);
    return;
  }

  // terminate the buffer with a null character
  buffer[bpos] = '\0';
  return;
}

}

namespace Format {

struct FormatArgument {
  virtual auto toString() const -> const char* = 0;
};

template <typename... Args>
constexpr void format(char *buffer, const char *string, Args... args) {
  formatImpl(buffer, string, 0, 0, args...);
}

}
