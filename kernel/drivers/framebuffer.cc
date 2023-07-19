#include <stddef.h>

#include <kernel/utils/halt.h>
#include <kernel/utils/panic.h>
#include <kernel/utils/color.h>
#include <kernel/drivers/io.h>

#include <lib/thirdparty/limine.h>
#include <assets/fonts/fonts.h>

#include "framebuffer.h"

struct Writer {
  u8 x_offset = 8;
  u8 y_offset = 4;

  u32 x = x_offset;
  u32 y = y_offset;
  u32 background = Color::encodeHEX(0x11111B);
  u32 foreground = Color::encodeHEX(0xD9E0EE);

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

    if (x >= framebuffer->width - x_offset - fc->charWidth) {
      writeNewLine();
    }
    
    for (int row=0; row<fc->maxHeight; row++) {
      u8 rowData = fc->data[row] >> (fc->maxWidth - fc->charWidth);
    
      for (int col=0; col<fc->charWidth; col++) {
        bool isPixelActive = (rowData >> (fc->charWidth - col - 1)) & 1;
    
        u32 pixelColor = isPixelActive ? foreground : background;
        writePixel(col + x, row + y, pixelColor);
      }
    
    }
    
    x += fc->charWidth + 2;
  };

  void writeNewLine() {
    x = x_offset;
    y += 16;
  };

  void writePixel(u32 posX, u32 posY, u32 color) {
    u64 offset = posY*framebuffer->pitch + posX*framebuffer->bpp/8;
    IO::Mem::write<u32>(this->base + offset, color);
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

void Framebuffer::withColor(u32 foreground, u32 background, void (*function)()) {
  u32 oldForeground = gWriter.foreground;
  u32 oldBackground = gWriter.background;

  gWriter.foreground = foreground;
  gWriter.background = background;
  function();
  gWriter.foreground = oldForeground;
  gWriter.background = oldBackground;
}

void Framebuffer::withForeground(u32 foreground, void (*function)()) {
  u32 oldForeground = gWriter.foreground;
  gWriter.foreground = foreground;
  function();
  gWriter.foreground = oldForeground;
}

void Framebuffer::withBackground(u32 background, void (*function)()) {
  u32 oldBackground = gWriter.background;
  gWriter.background = background;
  function();
  gWriter.background = oldBackground;
}


void Framebuffer::setForeground(u32 color) {
  gWriter.foreground = color;
}

void Framebuffer::setForeground(u8 r, u8 g, u8 b) {
  gWriter.foreground = Color::encodeRGB(r, g, b);
}

void Framebuffer::setBackground(u32 color) {
  gWriter.background = Color::encodeHEX(color);
}

void Framebuffer::setBackground(u8 r, u8 g, u8 b) {
  gWriter.background = Color::encodeRGB(r, g, b);
}

