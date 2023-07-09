#pragma once
#include <type_traits>
#include <stddef.h>

namespace Format {

template<typename T>
constexpr int convert_int_to_string(T value, char* buffer, int radix) {
  bool negative = false;
  
  if (value < 0) {
    negative = true;
    value = -value;
  }
  
  int length = 0;
  
  do {
    int remainder = value % radix;
    buffer[length++] = static_cast<char>((remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10));
    value /= radix;
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
  
  return length;
}

template<typename T>
[[nodiscard]] 
static constexpr auto append_value(char* buffer, size_t bpos, T value) -> int {
  if constexpr (std::is_same<const char*, T>::value) {
    int i=0;
    while (value[i] != '\0') {
      buffer[bpos] = value[i];
      bpos += 1;
      i++;
    }
    return i;
  }

  if constexpr (std::is_same<int, T>::value) {
    constexpr size_t MAX_INT_LENGTH = 11;  // Maximum length of an integer (including sign)
    char temp_buffer[MAX_INT_LENGTH];
    int length = convert_int_to_string(value, temp_buffer, 10); // Use base 10 as default
    
    // Copy the characters to the buffer
    for (int i = 0; i < length; i++) {
        buffer[bpos] = temp_buffer[i];
        bpos += 1;
    }
    
    return length;
  }

  return 0;
}

static constexpr void format_impl(char* buffer, const char* string, size_t bpos, size_t spos) {  

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
static constexpr void format_impl(char* buffer, const char* string, size_t bpos, size_t spos, Arg arg, Args ...args) {

  while (string[spos] != '\0') {
    if (string[spos] == '{' && string[spos + 1] == '}') {
      spos += 2;
      bpos += append_value(buffer, bpos, arg);
      format_impl(buffer, string, bpos, spos, args...);
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
  format_impl(buffer, string, 0, 0, arg, args...);
}

}
