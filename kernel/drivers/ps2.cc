#include <kernel/drivers/ps2.h>
#include <kernel/arch/x86_64/interrupt/pic.h>
#include <kernel/drivers/io.h>
#include <kernel/utils/print.h>

#include <kernel/arch/platform.h>

#include <limine.h>
#include <lib/types.h>
#include <optional>

struct Key {
  enum Type {

    // TODO:
    // Disambiguate between left/right ctrl/shift
    Shift,
    Ctrl,

    ShiftRelease,

    Esc,
    Tab,
    Alt,

    Backspace,
    Enter,

    NumLock,
    CapsLock,
    ScrollLock,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    // Need to handle this somehow
    ModSequence,
    
    Printable,

    Unset = 0xF
  };

  Type type = Unset;
  const char* input = "";
};

auto keyboardHandler() -> void;
auto parseScancode(u8 scancode) -> std::optional<Key>;

auto PS2::Keyboard::initialize() -> void {
  x86_64::PIC::clearMask(1);
  Arch::Interrupt::setIrqHandler(33, keyboardHandler);
}

auto keyboardHandler() -> void {
  x86_64::PIC::sendEndOfInterrupt(33);

  u8 scancode = IO::inb(0x60);

  std::optional<Key> key = parseScancode(scancode);

  if (!key) return;

  if (key->type == Key::Enter) {
    Framebuffer::writeNewLine();
    return;
  }
 
  // TODO:
  // We would need to allocate memory for this
  //
  // if (key->type == Key::Backspace) {
  //   Framebuffer::deleteLastChar();
  //   return;
  // }

  if (key->type != Key::Printable) return;

  Kernel::print(key->input);
}

auto parseScancode(u8 scancode) -> std::optional<Key> {
  static bool isShift = false;
  static bool isCapsLock = false;
  static bool isCtrl = false;
  static bool isAlt = false;
  static bool isNumLock = false;
  static bool isScrollLock = false;

  Key key{};

  // this is for the vast majority of cases
  key.type = Key::Printable;

  bool isUnrecognizedScancode = false;

  switch (scancode) {
    case 0x01:
      key.type = Key::Esc;
      key.input = "Esc";
      break;
    case 0x02:
      key.input = isShift ? "!" : "1";
      break;
    case 0x03:
      key.input = isShift ? "@" : "2";
      break;
    case 0x04:
      key.input = isShift ? "#" : "3";
      break;
    case 0x05:
      key.input = isShift ? "$" : "4";
      break;
    case 0x06:
      key.input = isShift ? "%" : "5";
      break;
    case 0x07:
      key.input = isShift ? "^" : "6";
      break;
    case 0x08:
      key.input = isShift ? "&" : "7";
      break;
    case 0x09:
      key.input = isShift ? "*" : "8";
      break;
    case 0x0A:
      key.input = isShift ? "(" : "9";
      break;
    case 0x0B:
      key.input = isShift ? ")" : "0";
      break;
    case 0x0C:
      key.input = isShift ? "-" : "_";
      break;
    case 0x0D:
      key.input = isShift ? "=" : "+";
      break;
    case 0x0E:
      key.type = Key::Backspace;
      key.input = "Backspace";
      break;
    case 0x0F:
      key.type = Key::Tab;
      key.input = "Tab";
      break;
    case 0x10:
      key.input = isCapsLock || isShift ? "Q" : "q";
      break;
    case 0x11:
      key.input = isCapsLock || isShift ? "W" : "w";
      break;
    case 0x12:
      key.input = isCapsLock || isShift ? "E" : "e";
      break;
    case 0x13:
      key.input = isCapsLock || isShift ? "R" : "r";
      break;
    case 0x14:
      key.input = isCapsLock || isShift ? "T" : "t";
      break;
    case 0x15:
      key.input = isCapsLock || isShift ? "Y" : "y";
      break;
    case 0x16:
      key.input = isCapsLock || isShift ? "U" : "u";
      break;
    case 0x17:
      key.input = isCapsLock || isShift ? "I" : "i";
      break;
    case 0x18:
      key.input = isCapsLock || isShift ? "O" : "o";
      break;
    case 0x19:
      key.input = isCapsLock || isShift ? "P" : "p";
      break;
    case 0x1A:
      key.input = isCapsLock || isShift ? "[" : "{";
      break;
    case 0x1B:
      key.input = isCapsLock || isShift ? "]" : "}";
      break;
    case 0x1C:
      key.type = Key::Enter;
      key.input = "Enter";
      break;
    case 0x1D:
      key.type = Key::Ctrl;
      key.input = "Ctrl";
      break;
    case 0x1E:
      key.input = isCapsLock || isShift ? "A" : "a";
      break;
    case 0x1F:
      key.input = isCapsLock || isShift ? "S" : "s";
      break;
    case 0x20:
      key.input = isCapsLock || isShift ? "D" : "d";
      break;
    case 0x21:
      key.input = isCapsLock || isShift ? "F" : "f";
      break;
    case 0x22:
      key.input = isCapsLock || isShift ? "G" : "g";
      break;
    case 0x23:
      key.input = isCapsLock || isShift ? "H" : "h";
      break;
    case 0x24:
      key.input = isCapsLock || isShift ? "J" : "j";
      break;
    case 0x25:
      key.input = isCapsLock || isShift ? "K" : "k";
      break;
    case 0x26:
      key.input = isCapsLock || isShift ? "L" : "l";
      break;
    case 0x27:
      key.input = isShift ? ":" : ";";
      break;
    case 0x28:
      key.input = isShift ? "\"" : "'";
      break;
    case 0x29:
      key.input = isShift ? "^" : "~";
      break;
    case 0x2A:
      isShift = true;
      key.type = Key::Shift;
      key.input = "Shift";
      break;
    case 0x2B:
      key.input = isShift ? "|" : "\\";
      break;
    case 0x2C:
      key.input = isCapsLock || isShift ? "Z" : "z";
      break;
    case 0x2D:
      key.input = isCapsLock || isShift ? "X" : "x";
      break;
    case 0x2E:
      key.input = isCapsLock || isShift ? "C" : "c";
      break;
    case 0x2F:
      key.input = isCapsLock || isShift ? "V" : "v";
      break;
    case 0x30:
      key.input = isCapsLock || isShift ? "B" : "b";
      break;
    case 0x31:
      key.input = isCapsLock || isShift ? "N" : "n";
      break;
    case 0x32:
      key.input = isCapsLock || isShift ? "M" : "m";
      break;
    case 0x33:
      key.input = isShift ? "<" : ",";
      break;
    case 0x34:
      key.input = isShift ? ">" : ".";
      break;
    case 0x35:
      key.input = isShift ? "?" : "/";
      break;
    case 0x36:
      isShift = true;
      key.input = "Shift";
      key.type = Key::Shift;
      break;
    case 0x37:
      key.input = "*";
      break;
    case 0x38:
      isAlt = true;
      key.type = Key::Alt;
      key.input = "Alt";
      break;
    case 0x39:
      key.input = " ";
      break;
    case 0x3A:
      isCapsLock = !isCapsLock;
      key.type = Key::CapsLock;
      key.input = "CapsLock";
      break;
    case 0x3B:
      key.type = Key::F1;
      key.input = "F1";
      break;
    case 0x3C:
      key.type = Key::F2;
      key.input = "F2";
      break;
    case 0x3D:
      key.type = Key::F3;
      key.input = "F3";
      break;
    case 0x3E:
      key.type = Key::F4;
      key.input = "F4";
      break;
    case 0x3F:
      key.type = Key::F5;
      key.input = "F5";
      break;
    case 0x40:
      key.type = Key::F6;
      key.input = "F6";
      break;
    case 0x41:
      key.type = Key::F7;
      key.input = "F7";
      break;
    case 0x42:
      key.type = Key::F8;
      key.input = "F8";
      break;
    case 0x43:
      key.type = Key::F9;
      key.input = "F9";
      break;
    case 0x44:
      key.type = Key::F10;
      key.input = "F10";
      break;
    case 0x45:
      isNumLock = !isNumLock;
      key.type = Key::NumLock;
      key.input = "NumLock";
      break;
    case 0x46:
      isScrollLock = !isScrollLock;
      key.type = Key::ScrollLock;
      key.input = "ScrollLock";
      break;
    case 0x47:
      isNumLock = !isNumLock;
      key.type = Key::NumLock;
      key.input = "NumLock";
      break;
    case 0x48:
      key.input = isNumLock ? "8" : "CurUp";
      break;
    case 0x49:
      key.input = isNumLock ? "9" : "PageUp";
      break;
    case 0x4A:
      key.input = "-";
      break;
    case 0x4B:
      key.input = isNumLock ?  "4" : "CurLeft";
      break;
    case 0x4C:
      if (isNumLock)
        key.input = "5";
      else
        isUnrecognizedScancode = true;
       break;
    case 0x4D:
      key.input = isNumLock ?  "4" : "CurLeft";
      break;
    case 0x4E:
      key.input = "+";
      break;
    case 0x50:
      key.input = isNumLock ? "2" : "CurDown";
      break;
    case 0x51:
      key.input = isNumLock ? "3" : "PageDown";
      break;
    case 0x52:
      key.input = isNumLock ? "0" : "Insert";
      break;
    case 0x53:
      key.input = isNumLock ? "0" : "Delete";
      break;
    case 0x57:
      key.input = "F11";
      key.type = Key::F11;
      break;
    case 0x58:
      key.input = "F10";
      key.type = Key::F10;
      break;
      break;
    case 0xAA:
      isShift = false;
      key.input = "LeftShiftRelease";
      key.type = Key::ShiftRelease;
      break;
    default:
      isUnrecognizedScancode = true;
      break;
  }


  if (isUnrecognizedScancode) 
    return std::nullopt;

  return key;
}
