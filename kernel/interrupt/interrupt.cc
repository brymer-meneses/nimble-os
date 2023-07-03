
#include "interrupt.h"

#include "../lib/itoa.h"
#include "../vga/vga.h"

constexpr uint8_t IDT_MAX_DESCRIPTIONS = 32;

struct idt_register {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

struct idt_descriptor {
  uint16_t isr_low;
  uint16_t kernel_cs;
  uint8_t ist;
  uint8_t attributes;
  uint16_t isr_mid;
  uint32_t isr_high;
  uint32_t reserved;
} __attribute__((packed));

struct cpu_status {
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;

  uint64_t vector_number;
  uint64_t error_code;

  uint64_t iret_rip;
  uint64_t iret_cs;
  uint64_t iret_flags;
  uint64_t iret_rsp;
  uint64_t iret_ss;
} __attribute__((packed));

__attribute__((aligned(0x10)))
static idt_descriptor IDT[256];

void interrupt::set_descriptor(uint8_t vector, uint64_t handler, uint8_t flags) {

  idt_descriptor* entry = &IDT[vector];

  entry->isr_low = handler & 0xFFFF;
  entry->kernel_cs = 0x08;
  entry->ist = 0;
  entry->attributes = flags;
  entry->isr_mid = ((uint64_t) handler >> 16) & 0xFFF;
  entry->isr_high = ((uint64_t) handler >> 32) & 0xFFFFFFFF;
  entry->reserved = 0;
}


extern uintptr_t isr_stub_table[];

void interrupt::initialize_idt() {

  idt_register idtr;
  idtr.base = (uintptr_t) &IDT[0];
  idtr.limit = 0xFFF;

  for (uint8_t vector = 0; vector < 32; vector++) {
    interrupt::set_descriptor(vector, (uint64_t) isr_stub_table[vector], 0x8E);
  }

  // the `lidt` opcode, caches the `idt_register`, that's why
  // it is fine for this to load in the stack, and get cleared 
  // after this function is executed.

  asm volatile ("lidt %0" :: "m" (idtr));      // load the new IDT
  asm volatile ("sti");                        // set the interrupt flag
}

extern "C" void interrupt_handler(cpu_status* context) {
  char buffer[3];

  std::itoa(context->vector_number, buffer, 10);

  vga::set_foreground(vga::color::LightRed);
  vga::print("EXCEPTION ");
  vga::print(buffer);
  vga::print(" ");
  vga::set_foreground(vga::color::White);


  switch (context->vector_number) {
    case 0:
      vga::println("Division by zero.");  
      break;
    case 13:
      vga::println("General protection fault.");  
      break;
    case 14:
      vga::println("Page fault.");  
      break;
    default:
      vga::println("Unexpected interrupt.");
      break;
  }

  vga::set_foreground(vga::color::Yellow);


  asm volatile ("cli");
  asm volatile ("hlt");
}
