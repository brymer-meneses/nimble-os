#pragma once
#include <lib/types.h>
#include <type_traits>


template<typename T>
concept IntegralType = std::is_integral_v<T>;

namespace IO {

  extern inline u8 inb(u16 portnum) {
    u8 data = 0;
    asm volatile ("inb %1, %0": "=a" (data) : "Nd" (portnum));
    return data;
  }

  extern inline void outb(u16 portnum, u8 data) {
    asm volatile ("outb %0, %1" :: "a" (data), "Nd" (portnum));
  }

  extern inline void wait() {
    outb(0x80, 0);
  }

  namespace Mem {

    template<IntegralType T>
    extern inline T read(uintptr_t address) {
      return *(T volatile*) address;
    }

    template<IntegralType T>
    extern inline void write(uintptr_t address, T value) {
      *(T volatile*) address = value;
    }

  }


}
