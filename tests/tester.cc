#include <kernel/drivers/serial.h>
#include <kernel/drivers/io.h>

#include <assets/themes/themes.h>

#include <algorithm>

#include "tester.h"

static constexpr u16 TEST_MAX = 100;

using tester::internal::TestFunction;

static TestFunction g_tests[TEST_MAX];

static u16 g_testCount = 0;
static u16 g_numTestPass = 0;
static bool g_didCurrentTestPass = true;

static constexpr u8 QEMU_PORT = 0xf4;
static constexpr u8 QEMU_EXIT_SUCCESS = 0x10;
static constexpr u8 QEMU_EXIT_FAILURE = 0x11;

using serial::Colored;

auto tester::main() -> void {
  std::sort(&g_tests[0], &g_tests[g_testCount],
            [](const TestFunction &a, const TestFunction &b) {
              return std::lexicographical_compare(
                a.suiteName, a.suiteName + std::strlen(a.suiteName), 
                b.suiteName, b.suiteName + std::strlen(b.suiteName));
            });

  serial::println("--------------------------");
  serial::println("Running {} tests...", g_testCount);
  
  const char* currentSuite = "";
  for (int i=0; i<g_testCount; i++) {

    const auto suite = g_tests[i].suiteName;
    if (std::strcmp(currentSuite, suite) != 0) {
      serial::println("{}", Colored(g_tests[i].suiteName, Colored::Yellow));
      currentSuite = suite;
    }

    serial::print("  {} ... ", g_tests[i].testName);

    g_tests[i].function();

    if (g_didCurrentTestPass) {
      serial::println("{}", Colored("[ok]", Colored::Green)); 
      g_numTestPass += 1;
    } else {
      serial::println("{}", Colored("[error]", Colored::Red)); 
    }
  }


  serial::println("\nSummary");
  serial::println("Test Passed: {}/{}", g_numTestPass, g_testCount);
  serial::println("--------------------------");

  if (g_testCount == g_numTestPass) {
    IO::outb(QEMU_PORT, QEMU_EXIT_SUCCESS);
  } else {
    IO::outb(QEMU_PORT, QEMU_EXIT_FAILURE);
  }
}

auto tester::internal::registerTest(TestFunction test) -> void {
  g_tests[g_testCount] = test;
  g_testCount += 1;
}

auto tester::internal::invokeTestFailure() -> void {
  g_didCurrentTestPass = false;
}

auto tester::internal::invokeTestSuccess() -> void {
  g_didCurrentTestPass = true;
}
