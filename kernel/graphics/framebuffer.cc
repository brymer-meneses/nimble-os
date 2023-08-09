#include <cstddef>

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

  auto writeCharacter(const char character) -> void {

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
        if (!isPixelActive) continue;
    
        writePixel(col + x, row + y, foreground);
      }
    
    }
    
    x += fc->charWidth + 2;
  };

  auto writeNewLine() -> void {
    x = x_offset;
    y += 16;
  };

  auto writePixel(const u32 posX, const u32 posY, const u32 color) -> void {
    u64 offset = posY*framebuffer->pitch + posX*framebuffer->bpp/8;
    IO::Mem::write<u32>(this->base + offset, color);
  };

  auto writeString(const char* string) -> void {
    for (int i=0; string[i] != '\0'; i++) {
      writeCharacter(string[i]);
    }
  };

  auto clearScreen() -> void {

    for (size_t y = 0; y < framebuffer->height; ++y) {
      for (size_t x = 0; x < framebuffer->width; ++x) {
        writePixel(x, y, background);
      }
    }
  }

  Writer() {

    static volatile auto framebuffer_request = limine_framebuffer_request {
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


auto Framebuffer::writeNewLine() -> void {
  gWriter.writeNewLine();
}

auto Framebuffer::writeCharacter(const char character) -> void {
  gWriter.writeCharacter(character);
}

auto Framebuffer::writeString(const char* string) -> void {
  gWriter.writeString(string);
}

auto Framebuffer::clearScreen() -> void {
  gWriter.clearScreen();
}

auto Framebuffer::getBackground() -> u32 {
  return gWriter.background;
}

auto Framebuffer::getForeground() -> u32 {
  return gWriter.foreground;
}

auto Framebuffer::setForeground(const u32 color) -> void {
  gWriter.foreground = color;
}

auto Framebuffer::setForeground(const u8 r, const u8 g, const u8 b) -> void {
  gWriter.foreground = Color::encodeRGB(r, g, b);
}

auto Framebuffer::setBackground(const u32 color) -> void {
  gWriter.background = Color::encodeHEX(color);
}

auto Framebuffer::setBackground(const u8 r, const u8 g, const u8 b) -> void {
  gWriter.background = Color::encodeRGB(r, g, b);
}

