#pragma once

#include <stdint.h>

namespace Color {

  /// Function to encode a hex color value into a 32-bit RGBA value
  constexpr uint32_t encodeHEX(uint32_t hex_value) {
    uint8_t red = (hex_value >> 16) & 0xFF;
    uint8_t green = (hex_value >> 8) & 0xFF;
    uint8_t blue = hex_value & 0xFF;
    uint8_t alpha = 255; // Assuming maximum alpha value (opaque)

    return (alpha << 24) | (red << 16) | (green << 8) | blue;
  }

  /// Function to encode the color into a 32-bit RGBA value
  constexpr uint32_t encodeRGB(uint8_t red, uint8_t green, uint8_t blue) {
    return (0xff << 24) | (red << 16) | (green << 8) | blue;
  }


}
