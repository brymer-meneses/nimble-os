#pragma once

namespace Tester {

  auto main() -> void;

  namespace Internal {
    struct TestFunction {
      const char* testName;
      const char* suiteName;
      void (*function)();
    };

    auto invokeTestFailure() -> void;
    auto invokeTestSuccess() -> void;
    auto registerTest(TestFunction test) -> void;
  }


}
