#pragma once

#include <lib/types.h>
#include <kernel/arch/arch.h>

namespace process {
  auto exit(u8 errno) -> void;

  constexpr size_t PROCESS_NAME_MAX_LENGTH = 256;
  enum class ProcessStatus {
    Ready,
    Running,
    Dead,
  };

  struct Process {
    size_t processId;
    ProcessStatus status;
    arch::cpu::Context* context = nullptr;
    Process* next = nullptr;
    u64* pageTable = nullptr;
    char name[PROCESS_NAME_MAX_LENGTH];
  };

}
