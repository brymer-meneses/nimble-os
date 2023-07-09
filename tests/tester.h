#pragma once

namespace Tester {


  void main();

  namespace Internal {
    struct TestFunction {
      const char* testName;
      const char* suiteName;
      void (*function)();
    };

    void invokeTestFailure();
    void invokeTestSuccess();
    void registerTest(TestFunction test);
  }


}
