#include "scheduler.h"
#include <kernel/arch/arch.h>
#include <kernel/utils/halt.h>
#include <kernel/arch/timer.h>
#include <kernel/memory/memory.h>
#include <kernel/utils/logger.h>

#include <lib/types.h>
#include <lib/libc.h>

using arch::cpu::Context;
using process::Process;
using process::ProcessStatus;
using process::PROCESS_NAME_MAX_LENGTH;

static Process* firstProcess = nullptr;
static Process* currentProcess = nullptr;

static auto deleteProcess(Process* process) -> void {
  auto* prevProcess = process->prev;
  auto* nextProcess = process->next;
  
  // are we at the beginning?
  if (prevProcess == nullptr) {
    // then we set the first process as the next node
    // since we're deleting the first one
    firstProcess = nextProcess;

    if (nextProcess != nullptr) {
      nextProcess->prev = nullptr;
      currentProcess = firstProcess;
    } else {
      currentProcess = nextProcess;
    }

  // are we at the end?
  } else if (nextProcess == nullptr) {
    currentProcess = prevProcess;
    currentProcess->next = nullptr;
  } else {
    prevProcess->next = nextProcess;
    nextProcess->prev = prevProcess;
  }

  delete process;
}

auto scheduler::addProcess(Process* process) -> void {
  log::debug("scheduler: added process `{}` with rip at {#0x16}", process->name, process->context->iret.rip);

  if (firstProcess == nullptr and currentProcess == nullptr) {
    firstProcess = process;
    currentProcess = process;
  } else {
    currentProcess->next = process;
    process->prev = currentProcess;

    currentProcess = process;
  }
}

auto scheduler::switchTask(Context* context) -> void {
  arch::interrupt::sendEOI(32);

  if (currentProcess != nullptr and currentProcess->status == ProcessStatus::Dead) {
    deleteProcess(currentProcess);
  }

  // we have no process to run
  if (firstProcess == nullptr) {
    kernel::halt();
  }

  // if it is currently running, we save the context
  if (currentProcess->status == ProcessStatus::Running) {
    currentProcess->context = context;
  }

  if (currentProcess->next != nullptr) {
    currentProcess = currentProcess->next;
  } else {
    // move to beginning if we reach the end
    currentProcess = firstProcess;
  }

  currentProcess->status = ProcessStatus::Running;
  arch::switchContext(currentProcess->context);
}

auto scheduler::getCurrentProcess() -> Process* {
  return currentProcess;
}

auto scheduler::initialize() -> void {
  arch::timer::setInterruptCallback(switchTask, 1000000000000000000);
  log::info("scheduler: successfully initialized");
}
