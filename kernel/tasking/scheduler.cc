
#include <kernel/arch/arch.h>
#include "scheduler.h"

using arch::interrupt::InterruptFrame;

enum class ProcessStatus {
  Ready,
  Running,
  Dead,
};

struct Process {
  ProcessStatus status;
  InterruptFrame* context;
  Process* next;
};

static Process* firstProcess = nullptr;
static Process* currentProcess = firstProcess;

static auto idle_main() -> void {
  while (true) {
    asm volatile ("hlt");
  }
}

auto scheduler::schedule() -> void {

}

auto scheduler::initialize() -> void {

}

