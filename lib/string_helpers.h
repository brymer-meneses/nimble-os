#include <type_traits>
#include <stddef.h>

namespace StringHelpers {
  
template<typename T>
constexpr int integralToString(T value, int base, char* buffer) {
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

constexpr auto appendToBuffer(char* buffer, size_t bpos, const char* string) -> size_t {
  size_t i=0;
  while (string[i] != '\0') {
    buffer[bpos] = string[i];
    bpos += 1;
    i++;
  }
  return i;
}

}
