#pragma once
#include "tester.h"
#include "../vga/vga.h"

#define TEST(test_suite, test_name) \
void test_##test_suite_##test_name(); \
__attribute__((constructor)) \
void register_##test_suite_##test_name() { \
  tester::internal::test_function test; \
  test.suite_name = #test_suite; \
  test.function_name = #test_name; \
  test.function = test_##test_suite_##test_name; \
  tester::internal::register_test(test); \
}; \
void test_##test_suite_##test_name()

#define ASSERT_EQ(left, right) \
  if (left != right) { \
    tester::internal::invoke_test_failure(); \
  };

#define ASSERT_NEQ(left, right) \
  if (left == right) { \
    tester::internal::invoke_test_failure(); \
  }

#define ASSERT(value) \
  if (!value) { \
    tester::internal::invoke_test_failure(); \
  }
