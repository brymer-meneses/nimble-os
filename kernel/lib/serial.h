#pragma once

#include <stdint.h>

namespace serial {
  
  int initialize();
  void println(const char* string, ...);
  void print(const char* string, ...);
  
}
