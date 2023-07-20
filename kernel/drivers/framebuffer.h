#pragma once
#include <lib/types.h>

namespace Framebuffer {
  
  auto writeString(const char* string) -> void;
  auto writeCharacter(const char character) -> void;
  auto writeNewLine() -> void;

  auto clearScreen() -> void;
  auto setBackground(const u32 color) -> void;
  auto setBackground(const u8 r, const u8 g, const u8 b) -> void;

  auto setForeground(const u32 color) -> void;
  auto setForeground(const u8 r, const u8 g, const u8 b) -> void;

  auto withColor(const u32 foreground, const u32 background, void (*function)()) -> void;
  auto withForeground(const u32 foreground, void (*function)()) -> void;
  auto withBackground(const u32 background, void (*function)()) -> void;

}
