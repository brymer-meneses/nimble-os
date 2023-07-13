#include <stddef.h>

#include <kernel/utils/halt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/color.h>

#include <lib/thirdparty/limine.h>
#include <assets/fonts/fonts.h>

#include "framebuffer.h"

struct Writer {
  uint8_t x_offset = 8;
  uint8_t y_offset = 4;

  uint32_t x = x_offset;
  uint32_t y = y_offset;
  uint32_t background = Color::encodeHEX(0x11111B);
  uint32_t foreground = Color::encodeHEX(0xD9E0EE);

  limine_framebuffer* volatile framebuffer;
  uintptr_t base;

  void writeCharacter(char character) {

    using Fonts::FontCharacter;
    if (character == ' ') {
      x += 4;
      return;
    }

    if (character == '\n') {
      writeNewLine();
      return;
    };
    
    std::optional<FontCharacter> fc = Fonts::PixelOperator::getBitmap(character);

    if (!fc.has_value()) {
      return;
    }
    
    for (int row=0; row<fc->maxHeight; row++) {
      uint8_t rowData = fc->data[row] >> (fc->maxWidth - fc->charWidth);
    
      for (int col=0; col<fc->charWidth; col++) {
        bool isPixelActive = (rowData >> (fc->charWidth - col - 1)) & 1;
    
        uint32_t pixelColor = isPixelActive ? foreground : background;
        writePixel(col + x, row + y, pixelColor);
      }
    
    }
    
    x += fc->charWidth + 2;
  };

  void writeNewLine() {
    x = x_offset;
    y += 16;
  };

  void writePixel(uint32_t posX, uint32_t posY, uint32_t color) {
    uint64_t offset = posY*framebuffer->pitch + posX*framebuffer->bpp/8;
    *(uint32_t*) (this->base + offset) = color;
  };

  void writeString(const char* string) {
    for (int i=0; string[i] != '\0'; i++) {
      writeCharacter(string[i]);
    }
  };

  void clearScreen() {

    for (size_t y = 0; y < framebuffer->height; ++y) {
      for (size_t x = 0; x < framebuffer->width; ++x) {
        writePixel(x, y, background);
      }
    }
  }

  Writer() {

    static volatile limine_framebuffer_request framebuffer_request = {
      .id = LIMINE_FRAMEBUFFER_REQUEST,
      .revision = 0
    };

    if (framebuffer_request.response == nullptr ||
        framebuffer_request.response->framebuffer_count < 1)
      Kernel::halt();

    this->framebuffer = framebuffer_request.response->framebuffers[0];
    this->base = (uintptr_t) framebuffer->address;

  }
};

static Writer gWriter;


void Framebuffer::writeNewLine() {
  gWriter.writeNewLine();
}

void Framebuffer::writeCharacter(char character) {
  gWriter.writeCharacter(character);
}

void Framebuffer::writeString(const char* string) {
  gWriter.writeString(string);
}

void Framebuffer::clearScreen() {
  gWriter.clearScreen();
}

void Framebuffer::withColor(uint32_t foreground, uint32_t background, void (*function)()) {
  uint32_t oldForeground = gWriter.foreground;
  uint32_t oldBackground = gWriter.background;

  gWriter.foreground = foreground;
  gWriter.background = background;
  function();
  gWriter.foreground = oldForeground;
  gWriter.background = oldBackground;
}

void Framebuffer::withForeground(uint32_t foreground, void (*function)()) {
  uint32_t oldForeground = gWriter.foreground;
  gWriter.foreground = foreground;
  function();
  gWriter.foreground = oldForeground;
}

void Framebuffer::withBackground(uint32_t background, void (*function)()) {
  uint32_t oldBackground = gWriter.background;
  gWriter.background = background;
  function();
  gWriter.background = oldBackground;
}


void Framebuffer::setForeground(uint32_t color) {
  gWriter.foreground = color;
}

void Framebuffer::setForeground(uint8_t r, uint8_t g, uint8_t b) {
  gWriter.foreground = Color::encodeRGB(r, g, b);
}

void Framebuffer::setBackground(uint32_t color) {
  gWriter.background = Color::encodeHEX(color);
}

void Framebuffer::setBackground(uint8_t r, uint8_t g, uint8_t b) {
  gWriter.background = Color::encodeRGB(r, g, b);
}

