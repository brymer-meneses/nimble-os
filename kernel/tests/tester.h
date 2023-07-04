#pragma once

namespace tester {

  void main();

  namespace internal {
    struct test_function {
      const char* function_name;
      const char* suite_name;
      void (*function)();
    };

    void invoke_test_failure();
    void register_test(test_function test);
    void continue_tests();
  }
}

