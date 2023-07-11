#pragma once

#include <kernel/utils/print.h>
#include <tests/tester.h>
#include <lib/libc.h>

#include <type_traits>

#define TEST(test_suite, test_name) \
void test_##test_suite##_##test_name(); \
__attribute__((constructor)) \
void register_##test_suite##_##test_name() { \
  Tester::Internal::TestFunction test; \
  test.suiteName = #test_suite; \
  test.testName = #test_name; \
  test.function = test_##test_suite##_##test_name; \
  Tester::Internal::registerTest(test); \
}; \
void test_##test_suite##_##test_name()

template<typename Arg1, typename Arg2>
void assertEq(Arg1 arg1, Arg2 arg2) {
  if constexpr (!std::is_same_v<Arg1, Arg2>) {
    Tester::Internal::invokeTestFailure();
  }


  if constexpr (std::is_same_v<Arg1, const char*>) {
    if (std::strcmp(arg1, arg2) == 0) {
      Tester::Internal::invokeTestSuccess();
      return;
    }
  }

  if constexpr (std::is_same_v<Arg1, char*>) {
    if (std::strcmp(arg1, arg2) == 0) {
      Tester::Internal::invokeTestSuccess();
      return;
    }
  }

  if (arg1 != arg2) {
    Tester::Internal::invokeTestFailure();
  }
}

template<typename Arg1, typename Arg2>
void assertNeq(Arg1 arg1, Arg2 arg2) {
  if constexpr (std::is_same_v<Arg1, Arg2>) {
    Tester::Internal::invokeTestFailure();
  }
  if (arg1 == arg2) {
    Tester::Internal::invokeTestFailure();
  }
}

template<typename Arg1>
void assert(Arg1 arg1) {
  if (arg1) {
    Tester::Internal::invokeTestSuccess();
  } else {
    Tester::Internal::invokeTestFailure();
  }
}
