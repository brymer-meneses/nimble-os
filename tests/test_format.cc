#include "format.h"
#include "prelude.h"

TEST(Format, NumberFormat) {

  char buffer[256];

  Format::format(buffer, "The meaning of life is: {}!", 42);

  assertEq(buffer, "The meaning of life is: 42!");
}

TEST(Format, NumberFormatUint) {

  char buffer[256];

  Format::format(buffer, "Helloooo {}", (uint16_t) 1);

  assertEq(buffer, "Helloooo 1");
}

TEST(Format, StringFormat) {

  char buffer[256];

  Format::format(buffer, "The quick brown {} jumped over the lazy {}.", "fox", "cat");

  assertEq(buffer, "The quick brown fox jumped over the lazy cat.");

}
