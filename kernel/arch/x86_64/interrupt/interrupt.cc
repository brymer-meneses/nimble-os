
#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>
#include <array>

#include "idt.h"
#include "interrupt.h"

using namespace x86_64;

static const std::array<const char*, 32> exceptionMessages = {
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

using Interrupt::ExceptionHandler;
using Interrupt::InterruptFrame;
using Interrupt::IrqHandler;

static std::array<ExceptionHandler, 32> exceptionHandlers;
static std::array<IrqHandler, 256 - 32> irqHandlers;

static auto dumpInterruptFrame(const InterruptFrame* context) -> void {
  u16 interruptNumber = context->vector_number;
  Kernel::println("----------------------------");
  Kernel::println("Received Exception {#0x1}", interruptNumber);
  Kernel::println("Description: {}", exceptionMessages[interruptNumber]);
  Kernel::println("Error Code : {#0x16}", context->error_code);
  Kernel::println("----------------------------");
  Kernel::println("rax : {#0x16}", context->rax);
  Kernel::println("rbx : {#0x16}", context->rbx);
  Kernel::println("rcx : {#0x16}", context->rcx);
  Kernel::println("rdx : {#0x16}", context->rdx);
  Kernel::println("rsi : {#0x16}", context->rsi);
  Kernel::println("rdi : {#0x16}", context->rdi);
  Kernel::println("r8 : {#0x16}", context->r8);
  Kernel::println("r9 : {#0x16}", context->r9);
  Kernel::println("r10 : {#0x16}", context->r10);
  Kernel::println("r11 : {#0x16}", context->r11);
  Kernel::println("r12 : {#0x16}", context->r12);
  Kernel::println("r13 : {#0x16}", context->r13);
  Kernel::println("r14 : {#0x16}", context->r14);
  Kernel::println("r15 : {#0x16}", context->r15);
  Kernel::println("rip : {#0x16}", context->iret_rip);
  Kernel::println("cs : {#0x16}", context->iret_cs);
  Kernel::println("flags : {#0x16}", context->iret_flags);
  Kernel::println("rsp: {#0x16}", context->iret_rsp);
  Kernel::println("ss : {#0x16}", context->iret_ss);
  Kernel::println("----------------------------");

  Kernel::halt();
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
    Kernel::println("Received interrupt {} that has no handler.",
                    interruptNumber);
  } else {
    handler();
  }

  return;
}

auto Interrupt::setExceptionHandler(const u16 interruptNumber, ExceptionHandler handler) -> void {

  if (exceptionHandlers[interruptNumber] != nullptr) {
    Kernel::println(
        "Overriding exception interrupt handler {} with a new function.",
        interruptNumber);
    return;
  }

  exceptionHandlers[interruptNumber] = handler;
}

auto Interrupt::setIrqHandler(const u16 interruptNumber, IrqHandler handler) -> void {

  if (irqHandlers[interruptNumber] != nullptr) {
    Kernel::println(
      "Overriding exception interrupt handler {} with a new function.",
      interruptNumber);
    return;
  }

  irqHandlers[interruptNumber] = handler;
}

auto Interrupt::initialize() -> void {
  irqHandlers.fill(nullptr);
  exceptionHandlers.fill(nullptr);
  Log::info("Initialized Interrupts");
}
