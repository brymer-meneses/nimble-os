
#include "scheduler.h"
#include <kernel/arch/arch.h>

using arch::interrupt::InterruptFrame;

enum class ProcessStatus {
  Ready,
  Running,
  Dead,
};

struct Process {
  ProcessStatus status;
  InterruptFrame *context;
  Process *next = nullptr;
};

static Process *firstProcess = nullptr;
static Process *currentProcess = nullptr;

static auto idleMain() -> void {
  while (true) {
    asm volatile("hlt");
  }
}

static auto createProcess() -> Process { return {}; }

static auto deleteProcess(Process *) -> void { return; }

static auto addProcess(Process *) -> void { return; }

auto scheduler::schedule() -> void {
  if (firstProcess == nullptr) {

  } else {
  }
}

auto scheduler::initialize() -> void {


}
