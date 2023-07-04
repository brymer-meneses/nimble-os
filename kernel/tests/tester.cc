#include "tester.h"
#include "stdint.h"
#include "../lib//itoa.h"
#include "../vga/vga.h"

using tester::internal::test_function;

static test_function tests[100];
static uint16_t test_count = 0;
static bool current_test_did_pass = true;

void tester::main() {
  char buffer[4];
  std::itoa(test_count, buffer, 10);
  vga::print("Running ");
  vga::print(buffer);
  vga::println(" tests...");

  tester::internal::continue_tests();
}

void tester::internal::continue_tests() {

  static uint16_t i = 0;

  while (i < test_count) {
    test_function test = tests[i];

    vga::set_foreground(vga::color::LightGray);
    vga::print(test.function_name);
    vga::print(" ... ");

    test.function();

    if (current_test_did_pass) {
      vga::set_foreground(vga::color::LightGreen);
      vga::println("[ok]");
    } else {
      vga::set_foreground(vga::color::LightRed);
      vga::println("[err]");
    }

    current_test_did_pass = true;

    vga::set_foreground(vga::color::Yellow);

    i += 1;
  }

}

void tester::internal::invoke_test_failure() {
  current_test_did_pass = false;
}

void tester::internal::register_test(test_function test) {
  tests[test_count] = test;
  test_count += 1;
}
