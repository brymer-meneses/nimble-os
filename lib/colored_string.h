#pragma once
#include <stdint.h>
#include "format.h"

/// TODO:
/// I need a queue to feed the framebuffer driver to 
/// print the string correctly.

namespace ColoredString {

  struct Colored : Format::FormatArgument {
    const char* string;
    uint32_t textColor;


    const char * toString() const override {
      return string;
    };


  };
}

