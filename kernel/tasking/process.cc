#include "process.h"
#include "scheduler.h"

auto process::exit(u8) -> void {
  auto* process = scheduler::getCurrentProcess();
  process->status = process::ProcessStatus::Dead;
  scheduler::switchTask(nullptr);
}
