#include "process.h"
#include "scheduler.h"
#include <lib/libc.h>

auto process::exit(u8) -> void {
  auto* process = scheduler::getCurrentProcess();
  process->status = process::ProcessStatus::Dead;
  scheduler::switchTask(nullptr);
}

using namespace process;

Process::Process(const char* processName, Function func, bool isUserAccessible) {

  static size_t currentProcessId = 0;

  context = new arch::cpu::Context(func, isUserAccessible);

  std::memcpy(name, processName, PROCESS_NAME_MAX_LENGTH);

  processId = currentProcessId++;

}
