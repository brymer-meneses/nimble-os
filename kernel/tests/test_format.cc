#include "prelude.h"
#include "../lib/format.h"

TEST(format, test_mixed_format) {
  char buffer[256];
  lib::format(buffer, "Hello {s}, meaning of life {i}.", "world", 42);
  const char* string = buffer;

  assert_eq(string, "Hello world, meaning of life 42.");
}

TEST(format, test_empty_format) {
  char buffer[256];
  lib::format(buffer, "Hello world");
  const char* string = buffer;

  assert_eq(string, "Hello world");
}
