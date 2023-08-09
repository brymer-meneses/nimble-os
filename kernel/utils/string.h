#include <type_traits>
#include <cstddef>

namespace String {
  
template<typename T>
constexpr auto fromIntegral(T value, char* buffer, int base) -> int {
  bool negative = false;

  if (value < 0) {
      negative = true;
      value = -value;
  }

  const char* digits = "0123456789ABCDEF"; // Digits for hexadecimal base

  int length = 0;

  do {
      buffer[length++] = digits[value % base]; // Use modulus operator with base
      value /= base;
  } while (value != 0);

  if (negative) {
      buffer[length++] = '-';
  }

  // Reverse the string in the buffer
  for (int i = 0, j = length - 1; i < j; i++, j--) {
      char temp = buffer[i];
      buffer[i] = buffer[j];
      buffer[j] = temp;
  }

  buffer[length] = '\0'; // Add null terminator

  return length;
}

constexpr auto append(char* buffer, size_t bpos, const char* string) -> size_t {
  size_t i=0;
  while (string[i] != '\0') {
    buffer[bpos] = string[i];
    bpos += 1;
    i++;
  }
  return i;
}

constexpr auto copy(char *buffer, size_t bufpos, const char *string,
                          size_t string_start, size_t string_end) -> int {

  size_t i = string_start;
  while (string[i] != '\0' && i < string_end) {
    buffer[bufpos] = string[i];
    bufpos += 1;
    i += 1;
  }

  buffer[bufpos] = '\0';
  return i;
}

}
