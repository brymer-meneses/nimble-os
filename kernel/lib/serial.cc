
#include <stdint.h>
#include <stdarg.h>

#include "lib/format.h"
#include "lib/serial.h"
#include "lib/io.h"
#include "lib/libc.h"

constexpr uint16_t PORT = 0x3f8; // COM1

int serial::initialize() {
  io::outb(PORT + 1, 0x00);
  io::outb(PORT + 3, 0x80);
  io::outb(PORT + 0, 0x03);
  io::outb(PORT + 1, 0x00);
  io::outb(PORT + 3, 0x03);
  io::outb(PORT + 2, 0xC7);
  io::outb(PORT + 4, 0x0B);
  io::outb(PORT + 4, 0x1E);
  io::outb(PORT + 0, 0xAE);
  
  if (io::inb(PORT + 0) != 0xAE) {
    return 1;
  }

  io::outb(PORT + 4, 0x0F);
  return 0;
}

void serial::println(const char* string, ...) {
  va_list args;
  va_start(args, string);
  char buffer[256];
  lib::internal::format_impl(buffer, string, args);

  for (size_t i=0; i<std::strlen(buffer); i++) {
    io::outb(PORT, buffer[i]);
  };

  io::outb(PORT, '\n');
  va_end(args);
}

void serial::print(const char* string, ...) {
  va_list args;
  va_start(args, string);
  char buffer[256];
  lib::internal::format_impl(buffer, string, args);

  for (size_t i=0; i<std::strlen(buffer); i++) {
    io::outb(PORT, buffer[i]);
  };

  va_end(args);
}
