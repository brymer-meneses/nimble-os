#pragma once
#include <lib/types.h>
#include <type_traits>


template<typename T>
concept Integral = std::is_integral_v<T>;

namespace IO {

  extern inline auto inb(u16 portnum) -> u8 {
    u8 data = 0;
    asm volatile ("inb %1, %0": "=a" (data) : "Nd" (portnum));
    return data;
  }

  extern inline auto outb(u16 portnum, u8 data) -> void  {
    asm volatile ("outb %0, %1" :: "a" (data), "Nd" (portnum));
  }

  extern inline auto wait() -> void {
    outb(0x80, 0);
  }

  namespace Mem {

    template<Integral T>
    extern inline auto read(uintptr_t address) -> T {
      return *(T volatile*) address;
    }

    template<Integral T>
    extern inline auto write(uintptr_t address, T value) -> void {
      *(T volatile*) address = value;
    }

  }


}
