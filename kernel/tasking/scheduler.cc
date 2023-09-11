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

static constexpr u8 KERNEL_CS = 0x8;
static constexpr u8 KERNEL_SS = 0x10;

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
    currentProcess = process;
  }
}

static auto deleteProcess(Process* process) -> void {
  if (process == firstProcess) {
    firstProcess = nullptr;
    currentProcess = nullptr;
  }
}

auto scheduler::switchTask(Context* context) -> void {
  arch::interrupt::sendEOI(32);

  if (currentProcess != nullptr and currentProcess->status == ProcessStatus::Dead) {
    deleteProcess(currentProcess);
  } else if (currentProcess != nullptr) {
    currentProcess->status = ProcessStatus::Running;
  }

  // we have no process to run
  if (firstProcess == nullptr) {
    kernel::halt();
  }

  if (firstProcess == currentProcess) {
    arch::switchContext(firstProcess->context);
  } 
  
  currentProcess->context = context;
  if (currentProcess->next == nullptr) {
    currentProcess = firstProcess;
  } else {
    currentProcess = currentProcess->next;
  }

  arch::switchContext(currentProcess->context);
}

auto scheduler::getCurrentProcess() -> Process* {
  return currentProcess;
}

auto scheduler::createKernelProcess(const char* name, Function func, FunctionArg arg) -> void {
  log::debug("scheduler: creating kernel process `{}` at func {#0x16}", name, func);

  auto* process = (Process*) kernel::malloc(sizeof(Process));
  process->next = nullptr;
  process->processId = currentProcessId++;
  process->status = ProcessStatus::Ready;
  std::memcpy(process->name, name, PROCESS_NAME_MAX_LENGTH);

  process->context = (arch::cpu::Context*) kernel::malloc(sizeof(arch::cpu::Context));
  process->context->iret.cs = KERNEL_CS;
  process->context->iret.ss = KERNEL_SS;
  process->context->iret.rip = (u64) func;
  process->context->iret.flags = 0x202;
  process->context->iret.rsp = (u64) kernel::malloc(4096);
  addProcess(process);
}

auto scheduler::initialize() -> void {
  arch::timer::setInterruptCallback(switchTask, 1000000000000000000);
  log::info("scheduler: successfully initialized");
}
