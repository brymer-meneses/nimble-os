#include <stddef.h>

#include "framebuffer.h"
#include "../lib/kernel/halt.h"
#include "../lib/limine.h"
#include "../lib/color.h"
#include "../assets/fonts/fonts.h"


static volatile limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};


static uint32_t g_backgroundColor = Color::encodeRGB(46, 52, 64);
static uint32_t g_foregroundColor = Color::encodeRGB(236, 239, 244);

static constexpr uint8_t offset = 10;
// static constexpr uint8_t lineSpacing = 3;

static uint64_t g_pos = offset;

void Framebuffer::writeNewLine() {
  const limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
  const uint64_t horizontal_offset = g_pos % framebuffer->width;
  
  static uint16_t row = 1;

  g_pos = framebuffer->width * 16 * row + offset;
  row += 1;
}

void Framebuffer::writeCharacter(char character) {

  Fonts::FontCharacter fc = Fonts::getPixelOperatorBitmap(character);

  const limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

  volatile uint32_t* vram = (uint32_t*) framebuffer->address;

  const uint32_t pixelWidth =  (framebuffer->pitch / 4);

  if (character == ' ') {
    g_pos += fc.charWidth + 2;
    return;
  }

  if (character == '\n') {
    writeNewLine();
    return;
  };


  // we can assume that each `fc` has the same height
  for (int row=0; row<fc.maxHeight; row++) {
    uint8_t rowData = fc.data[row] >> (fc.maxWidth - fc.charWidth);

    for (int col=0; col<fc.charWidth; col++) {
      bool isPixelActive = (rowData >> (fc.charWidth - col - 1)) & 1;

      uint32_t pixelColor = isPixelActive ? g_foregroundColor : g_backgroundColor;

      vram[ (row + offset) * pixelWidth + (col + g_pos) ] = pixelColor;
    }

  }

  g_pos += fc.charWidth + 2;
}

void Framebuffer::writeString(const char* string) {
  for (int i=0; string[i] != '\0'; i++) {
    writeCharacter(string[i]);
  }
}

void Framebuffer::clearScreen() {

  if (framebuffer_request.response == NULL
   || framebuffer_request.response->framebuffer_count < 1) {
    Kernel::halt();
  }

  limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

  // Assuming each pixel is represented by 32 bits (4 bytes)
  uint32_t* fb_ptr = (uint32_t*)framebuffer->address;
  size_t pixelsPerRow = framebuffer->pitch / 4;

  for (size_t y = 0; y < framebuffer->height; ++y) {
    for (size_t x = 0; x < framebuffer->width; ++x) {
      fb_ptr[y * pixelsPerRow + x] = g_backgroundColor;
    }
  }
}


void Framebuffer::setForeground(uint32_t color) {
  g_foregroundColor = Color::encodeHEX(color);
}

void Framebuffer::setForeground(uint8_t r, uint8_t g, uint8_t b) {
  g_foregroundColor = Color::encodeRGB(r, g, b);
}

void Framebuffer::setBackground(uint32_t color) {
  g_backgroundColor = Color::encodeHEX(color);
}

void Framebuffer::setBackground(uint8_t r, uint8_t g, uint8_t b) {
  g_backgroundColor = Color::encodeRGB(r, g, b);
}

