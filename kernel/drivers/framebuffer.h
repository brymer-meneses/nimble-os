#pragma once
#include <stdint.h>

namespace Framebuffer {
  
  void writeString(const char* string);
  void writeCharacter(char character);
  void writeNewLine();

  void clearScreen();
  void setBackground(uint32_t color);
  void setBackground(uint8_t r, uint8_t g, uint8_t b);

  void setForeground(uint32_t color);
  void setForeground(uint8_t r, uint8_t g, uint8_t b);

}
