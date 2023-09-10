#pragma once

namespace scheduler {
  auto initialize() -> void;

  using Function = void(*)(void*);
  using FunctionArg = void*;

  auto createKernelProcess(const char* name, Function func, FunctionArg arg) -> void;
  auto createUserProcess(const char* name, Function func, FunctionArg) -> void;
}
