
#include <kernel/utils/format.h>
#include <kernel/utils/print.h>
#include <kernel/utils/macros.h>
#include <lib/libc.h>

#include "interrupt.h"
#include "idt.h"

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
  "Reserved"
};


using Interrupt::IrqHandler;
using Interrupt::ExceptionHandler;
using Interrupt::InterruptFrame;

static ExceptionHandler exceptionHandlers[32];
static IrqHandler irqHandlers[256 - 32];

static auto dumpInterruptFrame(const InterruptFrame* context) -> void {
  u16 interruptNumber = context->vector_number;
  Framebuffer::setForeground(0xD08770);
  Kernel::println("----------------------------");
  Kernel::println("Received Exception {hex}", interruptNumber);
  Kernel::println("Description: {}", exceptionMessages[interruptNumber]);
  Kernel::println("Error Code : {hex}", context->error_code);
  Kernel::println("----------------------------");
  Kernel::println("rax : {hex}", context->rax);
  Kernel::println("rbx : {hex}", context->rbx);
  Kernel::println("rcx : {hex}", context->rcx);
  Kernel::println("rdx : {hex}", context->rdx);
  Kernel::println("rsi : {hex}", context->rsi);
  Kernel::println("rdi : {hex}", context->rdi);
  Kernel::println("r8 : {hex}",  context->r8);
  Kernel::println("r9 : {hex}",  context->r9);
  Kernel::println("r10 : {hex}", context->r10);
  Kernel::println("r11 : {hex}", context->r11);
  Kernel::println("r12 : {hex}", context->r12);
  Kernel::println("r13 : {hex}", context->r13);
  Kernel::println("r14 : {hex}", context->r14);
  Kernel::println("r15 : {hex}", context->r15);
  Kernel::println("rip : {hex}", context->iret_rip);
  Kernel::println("cs : {hex}", context->iret_cs);
  Kernel::println("flags : {hex}", context->iret_flags);
  Kernel::println("rsp: {hex}", context->iret_rsp);
  Kernel::println("ss : {hex}", context->iret_ss);
  Kernel::println("----------------------------");
  Framebuffer::setForeground(0xD8DEE9);
} 

// this is called from `idt.asm` 
extern "C" auto interruptDispatch(InterruptFrame* context) -> void {

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
    Kernel::println("Received interrupt {} that has no handler.",  interruptNumber);
  } else {
    handler();
  }

  return;
}

auto Interrupt::setExceptionHandler(const u16 interruptNumber, ExceptionHandler handler) -> void {

  if (exceptionHandlers[interruptNumber] != nullptr) {
    Kernel::println("Overriding exception interrupt handler {} with a new function.",  interruptNumber);
    return;
  }

  exceptionHandlers[interruptNumber] = handler;
}

auto Interrupt::setIrqHandler(const u16 interruptNumber, IrqHandler handler) -> void {

  if (irqHandlers[interruptNumber] != nullptr) {
    Kernel::println("Overriding exception interrupt handler {} with a new function.",  interruptNumber);
    return;
  }

  irqHandlers[interruptNumber] = handler;
}

auto Interrupt::initialize() -> void {
  std::memset(irqHandlers, 0, sizeof(irqHandlers));
  std::memset(exceptionHandlers, 0, sizeof(exceptionHandlers));
}
