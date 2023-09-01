#pragma once

#include <kernel/drivers/serial.h>
#include <tests/tester.h>
#include <lib/libc.h>

#include <type_traits>
#include <source_location>


#define TEST(test_suite, test_name) \
void test_##test_suite##_##test_name(); \
__attribute__((constructor)) \
void register_##test_suite##_##test_name() { \
  tester::internal::TestFunction test; \
  test.suiteName = #test_suite; \
  test.testName = #test_name; \
  test.function = test_##test_suite##_##test_name; \
  tester::internal::registerTest(test); \
}; \
void test_##test_suite##_##test_name() \

template<typename Arg1, typename Arg2>
auto assertEq(Arg1 arg1, Arg2 arg2) -> void {
  if constexpr (!std::is_same_v<Arg1, Arg2>) {
    tester::internal::invokeTestFailure();
  }


  if constexpr (std::is_same_v<Arg1, const char*>) {
    if (std::strcmp(arg1, arg2) == 0) {
      tester::internal::invokeTestSuccess();
      return;
    }
  }

  if constexpr (std::is_same_v<Arg1, char*>) {
    if (std::strcmp(arg1, arg2) == 0) {
      tester::internal::invokeTestSuccess();
      return;
    }
  }

  if (arg1 != arg2) {
    serial::println("Test failed");
    serial::println("Left {}", arg1);
    serial::println("Right {}", arg2);
    tester::internal::invokeTestFailure();
  } else {
    tester::internal::invokeTestSuccess();
  }

}

template<typename Arg1, typename Arg2>
auto assertNeq(Arg1 arg1, Arg2 arg2) -> void {
  if constexpr (std::is_same_v<Arg1, Arg2>) {
    tester::internal::invokeTestFailure();
  }
  if (arg1 == arg2) {
    tester::internal::invokeTestFailure();
  } else {
    tester::internal::invokeTestSuccess();
  }
}

template<typename Arg1>
auto assert(Arg1 arg1) -> void {
  if (arg1) {
    tester::internal::invokeTestSuccess();
  } else {
    tester::internal::invokeTestFailure();
  }
}
