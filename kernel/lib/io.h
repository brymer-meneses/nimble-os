#pragma once
#include <stdint.h>

namespace io {

  extern inline uint8_t inb(uint16_t portnum) {
    uint8_t data = 0;
    asm volatile("inb %0, %1" : "=a" (data) : "Nd"(portnum));
    return data;
  }

  extern inline void outb(uint16_t portnum, uint8_t data) {
    asm volatile("outb %1, %0" ::"a" (data), "Nd"(portnum));
  }

}
