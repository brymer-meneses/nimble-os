#include <lib/syslib/format.h>
#include <tests/prelude.h>

struct TestWriter : sl::FormatWriter {
  char buffer[1024];
  int i = 0;
  auto writeChar(const char character) -> void final {
    buffer[i] = character;
    buffer[i+1] = '\0';
    i += 1;
  };

  auto reset() -> void {
    i = 0;
  }
};

TestWriter writer{};

TEST(Format, NumberFormat) {
  writer.reset();

  sl::format(writer, "The meaning of life is: {}!", 42);
  assertEq(writer.buffer, "The meaning of life is: 42!");
}

TEST(Format, StringFormat) {
  writer.reset();

  sl::format(writer, "The quick brown {} jumped over the lazy {}.", "fox", "cat");
  assertEq(writer.buffer, "The quick brown fox jumped over the lazy cat.");
}

TEST(Format, Hex) {
  writer.reset();

  sl::format(writer, "{} TEST {#0x}-{}", "HEX", 314159265, 314159265);
  assertEq(writer.buffer, "HEX TEST 0x12B9B0A1-314159265");
}

TEST(Format, Binary) {
  writer.reset();

  sl::format(writer, "{}-{#0b}", 2048, 2048);
  assertEq(writer.buffer, "2048-0b100000000000");
}

TEST(Format, Octal) {
  writer.reset();

  sl::format(writer, "{}-{#0o}", 2048, 2048);
  assertEq(writer.buffer, "2048-0o4000");
}
