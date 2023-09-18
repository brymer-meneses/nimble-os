#pragma once

#include <lib/types.h>
#include <kernel/arch/arch.h>
#include "process.h"

namespace scheduler {
  auto initialize() -> void;

  using Function = void(*)();

  auto addProcess(process::Process* process) -> void;
  auto switchTask(arch::cpu::Context*) -> void;

  auto getCurrentProcess() -> process::Process*;
}
