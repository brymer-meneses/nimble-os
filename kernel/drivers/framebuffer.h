#pragma once
#include <lib/types.h>

namespace Framebuffer {
  
  void writeString(const char* string);
  void writeCharacter(char character);
  void writeNewLine();

  void clearScreen();
  void setBackground(u32 color);
  void setBackground(u8 r, u8 g, u8 b);

  void setForeground(u32 color);
  void setForeground(u8 r, u8 g, u8 b);

  void withColor(u32 foreground, u32 background, void (*function)());
  void withForeground(u32 foreground, void (*function)());
  void withBackground(u32 background, void (*function)());

}
