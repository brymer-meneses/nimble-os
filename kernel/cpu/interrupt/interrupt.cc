#include <stdint.h>
#include <lib/format.h>
#include <lib/kernel/print.h>
#include "interrupt.h"
#include "idt.h"
#include "lib/macros.h"
#include <lib/libc.h>

const char* exceptionMessages[] = {
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
  "VMM Communication Exception",
  "Security Exception",
  "Reserved",
  "Triple Fault",
};


using Interrupt::InterruptHandler;
using Interrupt::InterruptFrame;

static InterruptHandler handlers[IDT::MAX_ENTRIES];

// this is called from `idt.asm` 
extern "C" void interruptDispatch(InterruptFrame* context) {

  uint16_t interruptNumber = context->vector_number;

  if (interruptNumber < 32) {
    Kernel::println("Received Exception {}: {}", interruptNumber, exceptionMessages[interruptNumber]);
    return;
  }

  InterruptHandler handler = handlers[interruptNumber];

  // check if there is a handler available
  if (handler == nullptr) {
    Kernel::println("Received interrupt {} that has no handler.",  interruptNumber);
  } else {
    handler(context);
  }

  return;
}

void Interrupt::setHandler(uint16_t interruptNumber, InterruptHandler handler) {

  if (handlers[interruptNumber] != nullptr) {
    Kernel::println("Overriding interrupt handler {} with a new function.",  interruptNumber);
    return;
  }

  handlers[interruptNumber] = handler;

}

void Interrupt::initialize() {
  std::memset(handlers, 0, sizeof(handlers));
}
