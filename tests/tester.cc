#include <kernel/utils/print.h>
#include <assets/themes/themes.h>

#include "tester.h"

static constexpr u16 TEST_MAX = 100;

static Tester::Internal::TestFunction g_tests[TEST_MAX];
static u16 g_testCount = 0;
static u16 g_numTestPass = 0;
static bool g_didCurrentTestPass = false;

void Tester::main() {
  Kernel::println("--------------------------");
  // TODO: some number casting is going haywire here
  Kernel::println(">> Running {} tests...", g_testCount);
  
  for (int i=0; i<g_testCount; i++) {
    Kernel::print("{} ...  ", g_tests[i].testName);

    g_tests[i].function();

    if (g_didCurrentTestPass) {
      Framebuffer::withForeground(0xA3BE8C, []() {
        Kernel::println("[ok]");
      });
      g_numTestPass += 1;
    } else {
      Framebuffer::withForeground(0xBF616A, []() {
        Kernel::println("[error]");
      });
    }
  }

  Kernel::println(">> Summary");
  Kernel::println("Test Passed: {}/{}", g_numTestPass, g_testCount);
  Kernel::println("--------------------------");
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
