#pragma once

#include <lib/types.h>
#include <kernel/arch/arch.h>
#include "process.h"

namespace scheduler {
  auto initialize() -> void;

  using Function = void(*)();

  auto createKernelProcess(const char* name, Function func) -> void;
  auto createUserProcess(const char* name, Function func) -> void;

  auto switchTask(arch::cpu::Context*) -> void;

  auto getCurrentProcess() -> process::Process*;
}
