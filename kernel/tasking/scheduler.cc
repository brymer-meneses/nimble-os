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

static size_t currentProcessId = 0;

static auto addProcess(Process* process) -> void {
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

auto scheduler::createKernelProcess(const char* name, Function func) -> void {
  log::debug("scheduler: creating kernel process `{}` at func {#0x16}", name, func);

  auto* process = (Process*) kernel::malloc(sizeof(Process));
  process->next = nullptr;
  process->prev = nullptr;
  process->processId = currentProcessId++;
  process->status = ProcessStatus::Ready;
  std::memcpy(process->name, name, PROCESS_NAME_MAX_LENGTH);

  process->context = (arch::cpu::Context*) kernel::malloc(sizeof(arch::cpu::Context));
  arch::initializeContext(process->context, func, {.userAccessible=false, .writeable=true, .executable=true,});
  addProcess(process);
}

auto scheduler::initialize() -> void {
  arch::timer::setInterruptCallback(switchTask, 1000000000000000000);
  log::info("scheduler: successfully initialized");
}
