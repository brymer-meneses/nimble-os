#include <lib/types.h>
#include <lib/syslib/format.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>

#include "serial.h"
#include "io.h"

static constexpr u16 PORT = 0x3f8;

using SerialWriter = serial::SerialWriter;

auto SerialWriter::writeChar(const char character) -> void {
  if (character == '\0') {
    return;
  }
  IO::outb(PORT, character);
}

static SerialWriter writer;

auto serial::initialize() -> void {
  IO::outb(PORT + 1, 0x00);
  IO::outb(PORT + 3, 0x80);
  IO::outb(PORT + 0, 0x03);
  IO::outb(PORT + 1, 0x00);
  IO::outb(PORT + 3, 0x03);
  IO::outb(PORT + 2, 0xC7);
  IO::outb(PORT + 4, 0x0B);
  IO::outb(PORT + 4, 0x1E);
  IO::outb(PORT + 0, 0xAE);

  if (IO::inb(PORT + 0) != 0xAE) {
    kernel::halt();
  }

  IO::outb(PORT + 4, 0x0F);
  log::info("Initialized serial printing");
}

auto serial::getWriter() -> SerialWriter& {
  return writer;
}

using Colored = serial::Colored;

auto Colored::format(sl::FormatWriter& writer) -> void {
  const char* colorCode = "\x1b[0m";  // Default: Reset color at the end

  switch (mColor) {
    case Color::Black:
      colorCode = "\x1b[30m";
      break;
    case Color::Red:
      colorCode = "\x1b[31m";
      break;
    case Color::Green:
      colorCode = "\x1b[32m";
      break;
    case Color::Yellow:
      colorCode = "\x1b[33m";
      break;
    case Color::Blue:
      colorCode = "\x1b[34m";
      break;
    case Color::Magenta:
      colorCode = "\x1b[35m";
      break;
    case Color::Cyan:
      colorCode = "\x1b[36m";
      break;
    case Color::White:
      colorCode = "\x1b[37m";
      break;
  }

  writer.writeString(colorCode);
  writer.writeString(mData);
  writer.writeString("\x1b[0m");  // Reset color at the end
}

