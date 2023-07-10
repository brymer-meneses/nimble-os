#pragma once

#include <cstdint>
#include <cstddef>
#include <optional>


namespace Fonts {

  // 8 by 16 by
  struct FontCharacter {
    uint8_t charWidth;
    uint8_t maxWidth;
    uint8_t maxHeight;
    char character;
    uint8_t data[16];
  };


  namespace PixelOperator {
    std::optional<FontCharacter> getBitmap(char character);
  }


  
}
