#include "format.h"

auto sl::internal::format::formatImpl(sl::FormatWriter& writer, const char* string, size_t strPos) -> void { 
    while (string[strPos] != '\0') {
      writer.writeChar(string[strPos]);
      strPos += 1;
    }
  }

auto sl::internal::format::parseSpec(const char* string, size_t& strPos) -> std::optional<FormatSpec> {
  static constexpr auto isDigit = [](char character) -> bool {
    return '0' <= character and character <= '9';
  };

  // eat the '{'
  strPos += 1;
  FormatSpec spec;

  // if we get '{}' return the default spec
  if (string[strPos] == '}') {
    strPos += 1;
    return spec;
  }
  
  if (string[strPos] == '#') {
    spec.addBasePrefix = true;
    strPos += 1;
  }

  const auto paddingChar = string[strPos];
  if (paddingChar != '}' and paddingChar != '\0') {
    spec.paddingChar = paddingChar;
  }

  strPos += 1;
  const auto base = string[strPos];

  if (base != '}' and base != '\0') {
    strPos += 1;
    switch (base) {
      case 'b':
        spec.base = 2;
        break;
      case 'o':
        spec.base = 8;
        break;
      case 'x':
        spec.base = 16;
        break;
    }
  } else {
    return {};
  }

  if (string[strPos] == '}') {
    strPos += 1;
    return spec;
  }

  while (isDigit(string[strPos])) {
    const auto digit = string[strPos] - '0';
    spec.padding = spec.padding * 10 + digit;
    strPos += 1;
  }

  if (string[strPos] == '}') {
    strPos += 1;
    return spec;
  }

  return {};
}

