#pragma once
#include "format.h"
#include <lib/types.h>

/// TODO:
/// I need a queue to feed the framebuffer driver to 
/// print the string correctly.

namespace ColoredString {

  struct Colored : Format::FormatArgument {
    const char* string;
    u32 textColor;


    virtual auto toString() const -> const char* override {
      return string;
    };


  };
}

