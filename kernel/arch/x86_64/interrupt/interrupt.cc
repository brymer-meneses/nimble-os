
#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>
#include <array>

#include "idt.h"
#include "interrupt.h"

using namespace x86_64;

static const char* exceptionMessages[] = {
    "Division Error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device not Available",
    "Double Fault",
    "Reserved",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Aligment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"};

using interrupt::ExceptionHandler;
using interrupt::InterruptFrame;
using interrupt::IrqHandler;

static ExceptionHandler exceptionHandlers[32];
static IrqHandler irqHandlers[224];

static auto dumpInterruptFrame(const InterruptFrame* context) -> void {
  u16 interruptNumber = context->vector_number;

  log::warn("----------------------------");
  log::warn("Received Exception {#0x1}", interruptNumber);
  log::warn("Description: {}", exceptionMessages[interruptNumber]);
  log::warn("Error Code : {#0x16}", context->error_code);
  log::warn("----------------------------");
  log::warn("rax : {#0x16}", context->rax);
  log::warn("rbx : {#0x16}", context->rbx);
  log::warn("rcx : {#0x16}", context->rcx);
  log::warn("rdx : {#0x16}", context->rdx);
  log::warn("rsi : {#0x16}", context->rsi);
  log::warn("rdi : {#0x16}", context->rdi);
  log::warn("r8 : {#0x16}", context->r8);
  log::warn("r9 : {#0x16}", context->r9);
  log::warn("r10 : {#0x16}", context->r10);
  log::warn("r11 : {#0x16}", context->r11);
  log::warn("r12 : {#0x16}", context->r12);
  log::warn("r13 : {#0x16}", context->r13);
  log::warn("r14 : {#0x16}", context->r14);
  log::warn("r15 : {#0x16}", context->r15);
  log::warn("rip : {#0x16}", context->iret_rip);
  log::warn("cs : {#0x16}", context->iret_cs);
  log::warn("flags : {#0x16}", context->iret_flags);
  log::warn("rsp: {#0x16}", context->iret_rsp);
  log::warn("ss : {#0x16}", context->iret_ss);
  log::warn("----------------------------");

  kernel::halt();
}

// this is called from `idt.asm`
extern "C" auto interruptDispatch(InterruptFrame *context) -> void {
  u16 interruptNumber = context->vector_number;

  if (interruptNumber < 32) {
    ExceptionHandler handler = exceptionHandlers[interruptNumber];

    if (handler == nullptr) {
      dumpInterruptFrame(context);
    } else {
      handler(context);
    }
    return;
  }

  IrqHandler handler = irqHandlers[interruptNumber];

  // check if there is a handler available
  if (handler == nullptr) {
    log::warn("Received interrupt {} that has no handler.", interruptNumber);
  } else {
    handler();
  }

  return;
}

auto interrupt::setExceptionHandler(const u16 interruptNumber, ExceptionHandler handler) -> void {

  if (exceptionHandlers[interruptNumber] != nullptr) {
    log::warn("Overriding exception interrupt handler {} with a new function.",interruptNumber);
    return;
  }

  exceptionHandlers[interruptNumber] = handler;
}

auto interrupt::setIrqHandler(const u16 interruptNumber, IrqHandler handler) -> void {

  if (irqHandlers[interruptNumber] != nullptr) {
    log::warn("Overriding exception interrupt handler {} with a new function.", interruptNumber);
    return;
  }

  irqHandlers[interruptNumber] = handler;
}

auto interrupt::initialize() -> void {
  std::memset(&exceptionHandlers, 0, 32 * sizeof(ExceptionHandler));
  std::memset(&irqHandlers, 0, 224 * sizeof(ExceptionHandler));

  log::info("Initialized Interrupts");

  log::debug("Exception handlers at {#0x16} - {#0x16}", &exceptionHandlers, &exceptionHandlers[31]);
  log::debug("IRQ handlers at {#0x16} - {#0x16}", &irqHandlers, &irqHandlers[223]);
}
