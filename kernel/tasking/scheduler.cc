#include "scheduler.h"
#include <kernel/arch/arch.h>
#include <kernel/utils/halt.h>
#include <kernel/arch/timer.h>
#include <kernel/memory/memory.h>
#include <kernel/utils/logger.h>

#include <lib/types.h>
#include <lib/libc.h>

using arch::cpu::Context;

static constexpr size_t PROCESS_NAME_MAX_LENGTH = 256;
static constexpr u8 KERNEL_CS = 0x8;
static constexpr u8 KERNEL_SS = 0x10;

enum class ProcessStatus {
  Ready,
  Running,
  Dead,
};
struct Process {

  size_t processId;
  ProcessStatus status;
  Context* context = nullptr;
  Process* next = nullptr;
  u64* pageTable = nullptr;
  char name[PROCESS_NAME_MAX_LENGTH];
};

static Process* firstProcess = nullptr;
static Process* currentProcess = nullptr;
static size_t currentProcessId = 0;

static auto addProcess(Process* process) -> void {
  if (firstProcess == nullptr) {
    firstProcess = process;
    currentProcess = process;
  } else {
    currentProcess->next = process;
    currentProcess = process;
  }

  log::debug("scheduler: added process `{}` with rip at {#0x16}", process->name, process->context->iret.rip);
}

auto scheduler::createKernelProcess(const char* name, Function func, FunctionArg arg) -> void {
  log::debug("scheduler: creating kernel process `{}` at func {#0x16}", name, func);

  auto* process = (Process*) kernel::malloc(sizeof(Process));
  process->context = (arch::cpu::Context*) kernel::malloc(sizeof(arch::cpu::Context));

  std::memcpy(process->name, name, PROCESS_NAME_MAX_LENGTH);

  process->processId = currentProcessId++;

  process->status = ProcessStatus::Ready;

  VMFlag flags = {.userAccessible=true, .writeable=true, .executable=true};

  process->context->iret.cs = KERNEL_CS;
  process->context->iret.ss = KERNEL_SS;
  process->context->iret.rip = (u64) func;
  process->context->iret.flags = 0x202;
  process->context->rdi = (u64) arg;
  process->context->iret.rsp = (u64) memory::allocateStack(memory::getKernelPageMap(), flags);

  addProcess(process);
}


static auto deleteProcess(Process* prev, Process* current) -> void {

}

static auto switchTask(Context* context) -> void {
  arch::interrupt::sendEOI(32);
  serial::print(".");

  if (firstProcess == nullptr) {
    log::debug("scheduler: no process idling ...");
    kernel::halt();
  }

  // auto* prevProcesss = currentProcess;

  while (true) {
    currentProcess->context = context;
    if (currentProcess->next != nullptr) {
      currentProcess = currentProcess->next;
    } else {
      currentProcess = firstProcess;
    }

    // if (currentProcess != nullptr and currentProcess->status == ProcessStatus::Dead) {
    //   deleteProcess(prevProcesss, currentProcess);
    // } else {
    //   currentProcess->status = ProcessStatus::Running;
    //   break;
    // }
  }

  arch::switchContext(currentProcess->context);
}

auto scheduler::initialize() -> void {
  arch::timer::setInterruptCallback(switchTask, 10000000);
  log::info("scheduler: successfully initialized");
}
