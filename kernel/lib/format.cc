
#include <typeinfo>
#include <type_traits>
#include <stdarg.h>

#include "format.h"
#include "libc.h"
#include "itoa.h"


/// lib::format
///   an easy api to format string
///
/// Example:
///   char buffer[256];
///   format(buffer, "Hello {s} -- {i}", "world", 42);
///
///   ASSERT_EQ("Hello world -- 42", buffer);
///
/// TODO:
///   Implement float and other type support
void lib::format(char* buffer, const char* string, ...) {
  va_list args;
  va_start(args, string);

  size_t string_len = std::strlen(string);
  size_t string_pos = 0;
  size_t buffer_pos = 0;

  auto advance = [&]() mutable -> char {
    if (string_pos >= string_len) {
      return '\0';
    } 

    string_pos += 1;
    return string[string_pos-1];
  };

  auto match = [&](char expected) mutable -> bool {
    if (string[string_pos] == expected) {
      string_pos += 1;
      return true;
    };

    return false;
  };

  auto append_string = [&](const char* string_to_copy) mutable {
    size_t length = std::strlen(string_to_copy);
    for (size_t i=0; i<length; i++) {
      buffer[buffer_pos] = string_to_copy[i];
      buffer_pos += 1;
    }
  };

  auto append_char = [&](const char character) mutable {
    buffer[buffer_pos] = character;
    buffer_pos += 1;
  };


  while (string_pos < string_len) {

    char c = advance();

    // if we encounter "\{"
    if (c == '\\' && match('{')) {
      append_string("\{");
      continue;
    };

    if (c == '{') {
      c = advance();

      // if we don't match a bracket we assume that it is not a format argument
      if (!match('}')) {
        append_char('{');
        append_char(c);
        buffer_pos += 2;
        continue;
      }

      switch (c) {
        case 's': 
          append_string(va_arg(args, const char*));
          break;
        case 'i': 
          char num[16];
          itoa(va_arg(args, int), num, 10);
          append_string(num);
          break;
      };

      continue;
    }

    append_char(c);
  }
  va_end(args);

  append_char('\0');
  return;
}
