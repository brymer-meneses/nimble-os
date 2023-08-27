#include <lib/types.h>
#include <lib/syslib/format.h>
#include <kernel/utils/halt.h>

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
    Kernel::halt();
  }

  IO::outb(PORT + 4, 0x0F);
}

auto serial::getWriter() -> SerialWriter& {
  return writer;
}
