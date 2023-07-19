#include <kernel/drivers/ps2.h>
#include <kernel/drivers/apic/ioapic.h>
#include <kernel/drivers/io.h>
#include <kernel/cpu/interrupt/idt.h>
#include <kernel/cpu/interrupt/interrupt.h>
#include <kernel/utils/print.h>
#include <kernel/drivers/pic.h>

#include <limine.h>
#include <lib/types.h>
#include <optional>

struct Key {
  bool isModifier;
  const char* input;
};

auto keyboardHandler() -> void;
auto parseScancode(u32 scancode) -> std::optional<Key>;

auto PS2::Keyboard::initialize() -> void {

  PIC::initialize();
  PIC::clearMask(1);

  Interrupt::setIrqHandler(33, keyboardHandler);
}

auto keyboardHandler() -> void {
  PIC::sendEndOfInterrupt(33);

  u32 scancode = IO::inb(0x60);

  std::optional<Key> key = parseScancode(scancode);

  if (!key || key->isModifier) return;

  if (std::strcmp(key->input, "Enter") == 0) {
    Framebuffer::writeNewLine();
    return;
  }

  // TODO:
  // We would need to allocate memory for this
  // if (std::strcmp(key->input, "Backspace") == 0) {
  //    Framebuffer::deleteLastChar();
  // }

  Kernel::print(key->input);
}

auto parseScancode(u32 scancode) -> std::optional<Key> {
  static bool isShift = false;
  static bool isCapsLock = false;
  static bool isCtrl = false;
  static bool isAlt = false;
  static bool isNumLock = false;
  static bool isScrollLock = false;

  Key key;

  bool isUnrecognizedScancode = false;

  switch (scancode) {
    case 0x01:
      key.isModifier = true;
      key.input = "Esc";
      break;
    case 0x02:
      key.isModifier = false;
      key.input = isShift ? "!" : "1";
      break;
    case 0x03:
      key.isModifier = false;
      key.input = isShift ? "@" : "2";
      break;
    case 0x04:
      key.isModifier = false;
      key.input = isShift ? "#" : "3";
      break;
    case 0x05:
      key.isModifier = false;
      key.input = isShift ? "$" : "4";
      break;
    case 0x06:
      key.isModifier = false;
      key.input = isShift ? "%" : "5";
      break;
    case 0x07:
      key.isModifier = false;
      key.input = isShift ? "^" : "6";
      break;
    case 0x08:
      key.isModifier = false;
      key.input = isShift ? "&" : "7";
      break;
    case 0x09:
      key.isModifier = false;
      key.input = isShift ? "*" : "8";
      break;
    case 0x0A:
      key.isModifier = false;
      key.input = isShift ? "(" : "9";
      break;
    case 0x0B:
      key.isModifier = false;
      key.input = isShift ? ")" : "0";
      break;
    case 0x0C:
      key.isModifier = false;
      key.input = isShift ? "-" : "_";
      break;
    case 0x0D:
      key.isModifier = false;
      key.input = isShift ? "=" : "+";
      break;
    case 0x0E:
      key.isModifier = false;
      key.input = "Backspace";
      break;
    case 0x0F:
      key.isModifier = true;
      key.input = "Tab";
      break;
    case 0x10:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "Q" : "q";
      break;
    case 0x11:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "W" : "w";
      break;
    case 0x12:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "E" : "e";
      break;
    case 0x13:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "R" : "r";
      break;
    case 0x14:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "T" : "t";
      break;
    case 0x15:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "Y" : "y";
      break;
    case 0x16:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "U" : "u";
      break;
    case 0x17:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "I" : "i";
      break;
    case 0x18:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "O" : "o";
      break;
    case 0x19:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "P" : "p";
      break;
    case 0x1A:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "[" : "{";
      break;
    case 0x1B:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "]" : "}";
      break;
    case 0x1C:
      key.isModifier = false;
      key.input = "Enter";
      break;
    case 0x1D:
      key.isModifier = false;
      key.input = "Ctrl";
      break;
    case 0x1E:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "A" : "a";
      break;
    case 0x1F:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "S" : "s";
      break;
    case 0x20:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "D" : "d";
      break;
    case 0x21:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "F" : "f";
      break;
    case 0x22:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "G" : "g";
      break;
    case 0x23:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "H" : "h";
      break;
    case 0x24:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "J" : "j";
      break;
    case 0x25:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "K" : "k";
      break;
    case 0x26:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "L" : "l";
      break;
    case 0x27:
      key.isModifier = false;
      key.input = isShift ? ":" : ";";
      break;
    case 0x28:
      key.isModifier = false;
      key.input = isShift ? "\"" : "'";
      break;
    case 0x29:
      key.isModifier = false;
      key.input = isShift ? "^" : "~";
      break;
    case 0x2A:
      isShift = true;
      key.isModifier = true;
      key.input = "Shift";
      break;
    case 0x2B:
      key.isModifier = false;
      key.input = isShift ? "|" : "\\";
      break;
    case 0x2C:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "Z" : "z";
      break;
    case 0x2D:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "X" : "x";
      break;
    case 0x2E:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "C" : "c";
      break;
    case 0x2F:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "V" : "v";
      break;
    case 0x30:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "B" : "b";
      break;
    case 0x31:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "N" : "n";
      break;
    case 0x32:
      key.isModifier = false;
      key.input = isCapsLock || isShift ? "M" : "m";
      break;
    case 0x33:
      key.isModifier = false;
      key.input = isShift ? "<" : ",";
      break;
    case 0x34:
      key.isModifier = false;
      key.input =  isShift ? ">" : ".";
      break;
    case 0x35:
      key.isModifier = false;
      key.input = isShift ? "?" : "/";
      break;
    case 0x36:
      isShift = true;
      key.isModifier = true;
      key.input = "Shift";
      break;
    case 0x37:
      key.isModifier = false;
      key.input = "*";
      break;
    case 0x38:
      isAlt = true;
      key.isModifier = true;
      key.input = "Alt";
      break;
    case 0x39:
      key.isModifier = false;
      key.input = " ";
      break;
    case 0x3A:
      isCapsLock = !isCapsLock;
      key.isModifier = true;
      key.input = "CapsLock";
      break;
    case 0x3B:
      key.isModifier = false;
      key.input = "F1";
      break;
    case 0x3C:
      key.isModifier = false;
      key.input = "F2";
      break;
    case 0x3D:
      key.isModifier = false;
      key.input = "F3";
      break;
    case 0x3E:
      key.isModifier = false;
      key.input = "F4";
      break;
    case 0x3F:
      key.isModifier = false;
      key.input = "F5";
      break;
    case 0x40:
      key.isModifier = false;
      key.input = "F6";
      break;
    case 0x41:
      key.isModifier = false;
      key.input = "F7";
      break;
    case 0x42:
      key.isModifier = false;
      key.input = "F8";
      break;
    case 0x43:
      key.isModifier = false;
      key.input = "F9";
      break;
    case 0x44:
      key.isModifier = false;
      key.input = "F10";
      break;
    case 0x45:
      isNumLock = !isNumLock;
      key.isModifier = true;
      key.input = "NumLock";
      break;
    case 0x46:
      isScrollLock = !isScrollLock;
      key.isModifier = true;
      key.input = "ScrollLock";
      break;
    case 0x47:
      isNumLock = !isNumLock;
      key.isModifier = true;
      key.input = "NumLock";
      break;
    case 0x48:
      key.isModifier = false;
      key.input = isNumLock ? "8" : "CurUp";
      break;
    case 0x49:
      key.isModifier = false;
      key.input = isNumLock ? "9" : "PageUp";
      break;
    case 0x4A:
      key.isModifier = false;
      key.input = "-";
      break;
    case 0x4B:
      key.isModifier = false;
      key.input = isNumLock ?  "4" : "CurLeft";
      break;
    case 0x4C:
      if (!isNumLock) {
        isUnrecognizedScancode = true;
      } else {
       key.isModifier = false;
       key.input = "5";
      } break;
    case 0x4D:
      key.isModifier = false;
      key.input = isNumLock ?  "4" : "CurLeft";
      break;
    case 0x4E:
      key.isModifier = false;
      key.input = "+";
      break;
    case 0x50:
      key.isModifier = false;
      key.input = isNumLock ? "2" : "CurDown";
      break;
    case 0x51:
      key.isModifier = false;
      key.input = isNumLock ? "3" : "PageDown";
      break;
    case 0x52:
      key.isModifier = false;
      key.input = isNumLock ? "0" : "Insert";
      break;
    case 0x53:
      key.isModifier = false;
      key.input = isNumLock ? "0" : "Delete";
      break;
    case 0x57:
      key.isModifier = false;
      key.input = "F11";
      break;
    case 0x58:
      key.isModifier = false;
      key.input = "F10";
      break;
    case 0xE0:
      key.isModifier = false;
      key.input = "E0";
      break;
    case 0xAA:
      isShift = false;
      break;
  }


  if (isUnrecognizedScancode) 
    return std::nullopt;

  if (!key.isModifier) {
    isAlt = false;
  }
  return key;
}
