#include <type_traits>

namespace StringHelpers {
  
template<typename T>
constexpr int integralToString(T value, char* buffer) {
  bool negative = false;

  if (value < 0) {
      negative = true;
      value = -value;
  }

  int length = 0;

  do {
      buffer[length++] = '0' + (value % 10);
      value /= 10;
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

}
