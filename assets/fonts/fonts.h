#pragma once

#include <lib/types.h>
#include <cstddef>
#include <optional>


namespace Fonts {

  // 8 by 16 by
  struct FontCharacter {
    u8 charWidth;
    u8 maxWidth;
    u8 maxHeight;
    char character;
    u8 data[16];
  };


  namespace PixelOperator {
     auto getBitmap(char character) -> std::optional<FontCharacter>;
  }


  
}
