#pragma once
#include "tester.h"
#include "../vga/vga.h"
#include "../lib/libc.h"

template<typename Arg1, typename Arg2>
void assert_eq(Arg1 arg1, Arg2 arg2) {
  if (!std::is_same<Arg1, Arg2>::value) {
    tester::internal::invoke_test_failure();
  }


  if (std::is_same<Arg1, const char*>::value) {
    if (std::strcmp(arg1, arg2) == 0) {
      tester::internal::invoke_test_success();
      return;
    }
  }

  if (arg1 != arg2) {
    tester::internal::invoke_test_failure();
  }
}

template<typename Arg1, typename Arg2>
void assert_neq(Arg1 arg1, Arg2 arg2) {
  if constexpr (std::is_same<Arg1, Arg2>::value) {
    tester::internal::invoke_test_failure();
  }
  if (arg1 == arg2) {
    tester::internal::invoke_test_failure();
  }
}

template<typename Arg1>
void assert(Arg1 arg1) {
  if (arg1) {
    tester::internal::invoke_test_success();
  } else {
    tester::internal::invoke_test_failure();
  }
}

#define TEST(test_suite, test_name) \
void test_##test_suite##_##test_name(); \
__attribute__((constructor)) \
void register_##test_suite##_##test_name() { \
  tester::internal::test_function test; \
  test.suite_name = #test_suite; \
  test.function_name = #test_name; \
  test.function = test_##test_suite##_##test_name; \
  tester::internal::register_test(test); \
}; \
void test_##test_suite##_##test_name()

#define ASSERT_EQ(left, right) \
  assert_eq(left, right);

#define ASSERT_NEQ(left, right) \
  assert_neq(left, right);

#define ASSERT(value) \
  assert(left, right);


