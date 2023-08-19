#include <tests/prelude.h>
#include <lib/syslib/bit.h>

TEST(BitManipulation, Unset) {
  u8 value = 0b11111111;
  u8 expected = 0b11101111;
  assertEq(expected, sl::bit::unset(value, 4));
}

TEST(BitManipulation, Set) {
  u8 value = 0b11101111;
  u8 expected = 0b11111111;
  assertEq(expected, sl::bit::set(value, 4));
}

TEST(BitManipulation, Mask) {
  u8 value = 0b11111111;
  u8 expected = 0b0011110000;
  assertEq(expected, sl::bit::mask(value, 2, 4));
}

TEST(BitManipulation, UnsetLeft) {
  u8 value = 0b11111111;
  u8 expected = 0b00001111;
  assertEq(expected, sl::bit::unsetLeft(value, 4));
}

TEST(BitManipulation, UnsetRight) {
  u8 value = 0b11111111;
  u8 expected = 0b11111000;
  assertEq(expected, sl::bit::unsetRight(value, 3));
}
