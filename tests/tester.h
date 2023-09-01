#pragma once

namespace tester {

  auto main() -> void;

  namespace internal {
    struct TestFunction {
      const char* testName;
      const char* suiteName;
      void (*function)();
    };

    auto invokeTestFailure() -> void;
    auto invokeTestSuccess() -> void;
    auto registerTest(TestFunction test) -> void;
    auto registerInternalTest(TestFunction test) -> void;
  }


}
