#include <kernel/drivers/serial.h>
#include <kernel/drivers/io.h>

#include <assets/themes/themes.h>

#include "tester.h"

static constexpr u16 TEST_MAX = 100;

static Tester::Internal::TestFunction g_tests[TEST_MAX];

static u16 g_testCount = 0;
static u16 g_numTestPass = 0;
static bool g_didCurrentTestPass = true;

static constexpr u8 QEMU_PORT = 0xf4;
static constexpr u8 QEMU_EXIT_SUCCESS = 0x10;
static constexpr u8 QEMU_EXIT_FAILURE = 0x11;

auto Tester::main() -> void{
  Serial::println("--------------------------");
  // TODO: some number casting is going haywire here
  Serial::println(">> Running {} tests...", g_testCount);
  
  for (int i=0; i<g_testCount; i++) {
    Serial::print("{} ...  ", g_tests[i].testName);

    g_tests[i].function();

    if (g_didCurrentTestPass) {
      // [ok] in green
      Serial::println("\x1b[32m[ok]\x1b[0m"); 
      g_numTestPass += 1;
    } else {
      // [error] in red
      Serial::println("\x1b[31m[error]\x1b[0m");
    }
  }


  Serial::println(">> Summary");
  Serial::println("Test Passed: {}/{}", g_numTestPass, g_testCount);
  Serial::println("--------------------------");

  if (g_testCount == g_numTestPass) {
    IO::outb(QEMU_PORT, QEMU_EXIT_SUCCESS);
  } else {
    IO::outb(QEMU_PORT, QEMU_EXIT_FAILURE);
  }
}

auto Tester::Internal::registerTest(Tester::Internal::TestFunction test) -> void {
  g_tests[g_testCount] = test;
  g_testCount += 1;
}

auto Tester::Internal::invokeTestFailure() -> void {
  g_didCurrentTestPass = false;
}

auto Tester::Internal::invokeTestSuccess() -> void {
  g_didCurrentTestPass = true;
}
