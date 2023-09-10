
#include <kernel/utils/print.h>
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>
#include <kernel/arch/x86_64/interrupt/pic.h>

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

using interrupt::InterruptHandler;

static InterruptHandler handlers[256];

static auto dumpInterruptFrame(const arch::cpu::Context* context) -> void {
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
  log::warn("rip : {#0x16}", context->iret.rip);
  log::warn("cs : {#0x16}", context->iret.cs);
  log::warn("flags : {#0x16}", context->iret.flags);
  log::warn("rsp: {#0x16}", context->iret.rsp);
  log::warn("ss : {#0x16}", context->iret.ss);
  log::warn("----------------------------");
  kernel::halt();
}

// this is called from `idt.asm`
extern "C" auto interruptDispatch(arch::cpu::Context *context) -> void {
  // log::debug("interrupt: received interrupt vector {}", context->vector_number);
  u16 interruptNumber = context->vector_number;

  if (interruptNumber < 32) {
    InterruptHandler handler = handlers[interruptNumber];

    if (handler == nullptr) {
      dumpInterruptFrame(context);
    } else {
      handler(context);
    }

    return;
  }

  InterruptHandler handler = handlers[interruptNumber];

  // check if there is a handler available
  if (handler == nullptr) {
    log::warn("Received interrupt {} that has no handler.", interruptNumber);
  } else {
    handler(context);
  }

}

auto interrupt::setInterruptHandler(const u16 interruptNumber, interrupt::InterruptHandler handler) -> void {
  if (handlers[interruptNumber] != nullptr) {
    log::warn("Overriding handler {}", interruptNumber);
  } 

  handlers[interruptNumber] = handler;
  if (interruptNumber >= 32) {
    x86_64::PIC::clearMask(interruptNumber);
  }

  log::debug("Added handler for interrupt vector {}", interruptNumber);
}

auto interrupt::initialize() -> void {
  std::memset(&handlers, 0, 256 * sizeof(InterruptHandler));

  IDT::initialize();
  log::debug("IRQ handlers at {#0x16} - {#0x16}", &handlers, &handlers[256]);

  interrupt::enable();
  log::info("Initialized Interrupts");
}

auto interrupt::sendEOI(const u8 interrupt) -> void {
  PIC::sendEOI(interrupt);
}

auto interrupt::disable() -> void {
  asm volatile ("cli");
}

auto interrupt::enable() -> void {
  asm volatile ("sti");
}
