#include "vga.h"
#include "lib/format.h"

#include <stdarg.h>

using vga::color;

constexpr uint8_t SCREEN_HEIGHT = 20;
constexpr uint8_t SCREEN_WIDTH = 80;
constexpr uint8_t MAX_SCREEN_HEIGHT = 100;
constexpr uint8_t TAB_SIZE = 4;

constexpr uint8_t encode_color(color fg, color bg) {
  return ((uint8_t) bg << 4) | ((uint8_t) fg);
}

struct character {
  uint8_t character;
  uint8_t attribute;
} __attribute__((packed)); 

struct screen {
  character characters[SCREEN_HEIGHT][SCREEN_WIDTH];
} __attribute__((packed));

class writer {

public:
  volatile screen* buffer = (screen*) 0xb8000;

  color foreground = color::Yellow;
  color background = color::Black;
  uint8_t column_position = 0;
  uint8_t row_position = 0;

public:
  void write_string(const char* string) volatile {
    uint16_t i = 0;

    while (string[i] != '\0') {
      this->write_character(string[i]);
      i++;
    }
  };

  void write_character(const char character) volatile {

    if (character == '\n') {
      this->write_new_line();
      return;
    }

    if (character == '\t') {
      for (uint8_t i=0; i<TAB_SIZE; i++) {
        this->write_character(' ');
      }
      return;
    }

    if (column_position >= SCREEN_WIDTH) {
      this->write_new_line();
    }

    uint8_t row = this->row_position;
    uint8_t col = this->column_position;

    this->buffer->characters[row][col].character = character;
    this->buffer->characters[row][col].attribute = encode_color(this->foreground, this->background);

    this->column_position += 1;
  };

  void write_new_line() volatile {
    this->column_position = 0;
    this->row_position += 1;

    if (this->row_position >= SCREEN_HEIGHT) {
      this->row_position = SCREEN_HEIGHT - 1;

      // move the screen up
      for (uint8_t row=1; row < SCREEN_HEIGHT; row++) {
        for (uint8_t col=0; col < SCREEN_WIDTH; col++) {
          this->buffer->characters[row-1][col].character = this->buffer->characters[row][col].character;
          this->buffer->characters[row-1][col].attribute = this->buffer->characters[row][col].attribute;
        }
      }

      this->clear_row(SCREEN_HEIGHT -1);
    };
  }

  void clear_row(uint8_t row) volatile {
    for (uint8_t col=0; col<SCREEN_WIDTH; col++) {
      this->buffer->characters[row][col].character = ' ';
      this->buffer->characters[row][col].attribute = encode_color(this->foreground, this->background);
    }
  };

  void clear_screen() volatile {
    for (uint8_t row=0; row<SCREEN_HEIGHT; row++) {
      clear_row(row);
    }
  }
};


static volatile writer WRITER{};

void vga::clear_screen() {
  WRITER.clear_screen();
}

void vga::print(const char* string, ...) {
  va_list args;
  va_start(args, string);

  char buffer[256];
  lib::internal::format_impl(buffer, string, args);
  WRITER.write_string(buffer);

  va_end(args);
}

void vga::println(const char* string, ...) {
  va_list args;
  va_start(args, string);

  char buffer[256];
  lib::internal::format_impl(buffer, string, args);
  WRITER.write_string(buffer);
  WRITER.write_new_line();

  va_end(args);
}

void vga::set_color(color fg, color bg) {
  WRITER.foreground = fg;
  WRITER.background = bg;
}

void vga::set_foreground(color fg) {
  WRITER.foreground = fg;
}

void vga::set_background(color bg) {
  WRITER.background = bg;
}
