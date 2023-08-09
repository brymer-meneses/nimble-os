#pragma once
#include <lib/types.h>
#include <type_traits>

namespace Framebuffer {
  
  auto writeString(const char* string) -> void;
  auto writeCharacter(const char character) -> void;
  auto writeNewLine() -> void;

  auto clearScreen() -> void;
  auto setBackground(const u32 color) -> void;
  auto setBackground(const u8 r, const u8 g, const u8 b) -> void;

  auto setForeground(const u32 color) -> void;
  auto setForeground(const u8 r, const u8 g, const u8 b) -> void;

  auto getForeground() -> u32;
  auto getBackground() -> u32;

  template<typename Function>
  concept FunctionType = std::is_invocable_v<Function>;

  template<FunctionType Function>
  auto withColor(const u32 foreground, const u32 background, Function function) -> void {
    u32 oldForeground = getForeground();
    u32 oldBackground = getBackground();
    setForeground(foreground);
    setForeground(background);
    function();
    setForeground(oldForeground);
    setForeground(oldBackground);
  }

  template<FunctionType Function>
  auto withForeground(const u32 foreground, Function function) -> void {
    u32 oldForeground = getForeground();
    setForeground(foreground);
    function();
    setForeground(oldForeground);
  }

  template<FunctionType Function>
  auto withBackground(const u32 background, Function function) -> void {
    u32 oldForeground = getBackground();
    setForeground(background);
    function();
    setForeground(oldForeground);
  }

}
