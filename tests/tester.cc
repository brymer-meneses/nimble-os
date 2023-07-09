#include <stdint.h>
#include <lib/kernel/print.h>

#include "tester.h"

static constexpr uint16_t TEST_MAX = 100;

static Tester::Internal::TestFunction g_tests[TEST_MAX];
static uint16_t g_testCount = 0;
static bool g_didCurrentTestPass = false;

void Tester::main() {
  Kernel::println("Running {} tests...", (int) g_testCount);
  
  for (int i=0; i<g_testCount; i++) {
    Kernel::print("{} ... ", g_tests[i].testName);

    g_tests[i].function();

    if (g_didCurrentTestPass) {
      Kernel::println("[ok]");
    } else {
      Kernel::println("[err]");
    }
  }

  Kernel::println("Tests Done!");
}

void Tester::Internal::registerTest(Tester::Internal::TestFunction test) {
  g_tests[g_testCount] = test;
  g_testCount += 1;
}

void Tester::Internal::invokeTestFailure() {
  g_didCurrentTestPass = false;
}

void Tester::Internal::invokeTestSuccess() {
  g_didCurrentTestPass = true;
}
