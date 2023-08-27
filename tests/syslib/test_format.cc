#include <lib/syslib/format.h>
#include <tests/prelude.h>

// currently broken :<

// TEST(Format, NumberFormat) {
//
//   char buffer[256];
//
//   sl::format(buffer, "The meaning of life is: {}!", 42);
//
//   assertEq(buffer, "The meaning of life is: 42!");
// }
//
// TEST(Format, StringFormat) {
//
//   char buffer[256];
//
//   sl::format(buffer, "The quick brown {} jumped over the lazy {}.", "fox", "cat");
//
//   assertEq(buffer, "The quick brown fox jumped over the lazy cat.");
//
// }
//
// TEST(Format, Hex) {
//   char buffer[256];
//   sl::format(buffer, "{}-{hex}", 314159265, 314159265);
//   assertEq(buffer, "314159265-0x12B9B0A1");
//   sl::format(buffer, "{} TEST {hex}-{}", "HEX", 314159265, 314159265);
//   assertEq(buffer, "HEX TEST 0x12B9B0A1-314159265");
// }
//
// TEST(Format, Binary) {
//   char buffer[256];
//
//   sl::format(buffer, "{}-{bin}", 2048, 2048);
//   assertEq(buffer, "2048-0b100000000000");
// }
//
// TEST(Format, Octal) {
//   char buffer[256];
//
//   sl::format(buffer, "{}-{oct}", 2048, 2048);
//   assertEq(buffer, "2048-0o4000");
// }
