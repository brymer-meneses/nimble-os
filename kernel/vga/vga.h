#pragma once

#include <stdint.h>

namespace vga {

enum class color : uint8_t {
  Black = 0,
  Blue = 1,
  Green = 2,
  Cyan = 3,
  Red = 4,
  Magenta = 5,
  Brown = 6,
  LightGray = 7,
  DarkGray = 8,
  LightBlue = 9,
  LightGreen = 10,
  LightCyan = 11,
  LightRed = 12,
  Pink = 13,
  Yellow = 14,
  White = 15,
};


void print(const char* string);
void println(const char* string);
void clear_screen();

void set_color(color fg, color bg);
void set_background(color bg);
void set_foreground(color fg);
}

