#pragma once
#include <cstddef>
#include <optional>
#include <type_traits>
#include <lib/libc.h>

#include <lib/syslib/string.h>
#include <lib/types.h>
#include <lib/libc.h>

namespace sl {
  struct FormatWriter {
    virtual auto writeChar(const char character) -> void = 0;
    virtual auto writeString(const char* string) -> void {
      for (size_t i = 0; string[i] != '\0'; i++) {
        writeChar(string[i]);
      }
    }
  };
}

namespace {

  struct FormatSpec {
    u8 base = 10;
    u8 padding = 0;
    char paddingChar = '0';
    u8 precision = 0;
    bool forcePlusSign = false;
    bool addBasePrefix = false;
  };

  template <typename T>
  inline auto formatArg(sl::FormatWriter& writer, const FormatSpec& spec, T arg) -> void { 
    constexpr auto isString = std::is_same_v<T, const char*> or std::is_same_v<T, char*>;

    if constexpr (isString) {
      writer.writeString(arg);
    } else if constexpr (std::is_pointer_v<T> and not isString) {

      // pass to the number formatter
      formatArg(writer, spec, (u64)(u64*) arg);
    } else if constexpr (std::is_integral_v<T>) {

      auto number = arg;
      if (number == 0) {
        writer.writeChar('0');
        return;
      }

      static constexpr auto digits = "0123456789ABCDEF";

      // if (number < 0) {
      //   number *= -1;
      //   writer.writeChar('-');
      // }

      if (spec.addBasePrefix) {
        switch (spec.base) {
          case 2:
            writer.writeString("0b");
            break;
          case 8:
            writer.writeString("0o");
            break;
          case 16:
            writer.writeString("0x");
            break;
        }
      }

      char buffer[64];

      u8 width = 0;
      while (number > 0) {
        const auto digit = number % spec.base;
        buffer[width] = digits[digit];
        width += 1;
        number /= spec.base;
      }

      for (u8 i = width; i < spec.padding; i++) {
        writer.writeChar(spec.paddingChar);
      }

      for (u8 i = width; i > 0; i--) {
        writer.writeChar(buffer[i-1]);
      }

    } 
  }

  inline auto formatImpl(sl::FormatWriter& writer, const char* string, size_t strPos) -> void { 
    while (string[strPos] != '\0') {
      writer.writeChar(string[strPos]);
      strPos += 1;
    }
  }
  
  inline auto parseSpec(const char* string, size_t& strPos) -> std::optional<FormatSpec> {
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

    return spec;
  }

  template <typename Arg, typename ...Args>
  auto formatImpl(sl::FormatWriter& writer, const char* string, size_t strPos, Arg arg, Args... args) -> void {
    while (string[strPos] != '\0') {
      if (string[strPos] == '{') {
        auto spec = parseSpec(string, strPos);

        if (!spec) { continue; }

        formatArg(writer, spec.value(), arg);
        formatImpl(writer, string, strPos, args...);
        return;
      }

      writer.writeChar(string[strPos]);
      strPos += 1;
    }
  }
}

namespace sl {
  template <typename ...Args>
  auto format(FormatWriter& writer, const char* string, Args... args) -> void {
    formatImpl(writer, string, 0, args...);
  }

}
