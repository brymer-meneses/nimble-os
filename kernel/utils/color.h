#pragma once

#include <lib/types.h>

namespace Color {

  /// Function to encode a hex color value into a 32-bit RGBA value
  constexpr auto encodeHEX(u32 hex_value) -> u32 {
    u8 red = (hex_value >> 16) & 0xFF;
    u8 green = (hex_value >> 8) & 0xFF;
    u8 blue = hex_value & 0xFF;
    u8 alpha = 255; // Assuming maximum alpha value (opaque)

    return (alpha << 24) | (red << 16) | (green << 8) | blue;
  }

  /// Function to encode the color into a 32-bit RGBA value
  constexpr auto encodeRGB(u8 red, u8 green, u8 blue) -> u32 {
    return (0xff << 24) | (red << 16) | (green << 8) | blue;
  }


}
