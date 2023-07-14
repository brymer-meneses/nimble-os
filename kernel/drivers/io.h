#pragma once
#include <lib/types.h>

namespace IO {

  extern inline u8 inb(u16 portnum) {
    u8 data = 0;
    asm volatile ("inb %0, %1": "=a" (data) : "Nd" (portnum));
    return data;
  }

  extern inline void outb(u16 portnum, u8 data) {
    asm volatile ("outb %1, %0l" :: "a" (data), "Nd" (portnum));
  }

}
