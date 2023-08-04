#include <kernel/utils/print.h>
#include <kernel/utils/panic.h>

#include "idt.h"

// NOTE:
// I once can't get interrupt working because `isr_high` is set to `16` bits
// Qwinci from OSDev helped me sort it out.
struct IdtEntry {
  u16 isr_low;
  u16 kernel_code_segment;
  u8 ist;
  u8 attributes;
  u16 isr_mid;
  u32 isr_high;
  u32 reserved;
} __attribute__((packed));

struct IdtPtr {
  u16 limit;
  u64 base;
} __attribute__((packed));

static constexpr u16 KERNEL_CODE_SEGMENT = 0x08;

__attribute__((aligned(0x10))) 
static IdtEntry idt[x86_64::IDT::MAX_ENTRIES];

static auto setEntry(u8 vector, u64 handler, u8 flags) -> void {

  idt[vector].isr_low = handler & 0xFFFF;
  idt[vector].isr_mid = (handler >> 16) & 0xFFFF;
  idt[vector].isr_high = (handler >> 32) & 0xFFFFFFFF;

  idt[vector].kernel_code_segment = KERNEL_CODE_SEGMENT;
  idt[vector].ist = 0;
  idt[vector].attributes = flags;
  idt[vector].reserved = 0;
}

extern void* interruptHandlerTable[];

auto x86_64::IDT::initialize() -> void {
  IdtPtr idtptr;

  idtptr.base = (u64) &idt[0];

  // "The reason for subtracting one from the size of the idt is interesting.
  // Loading an IDT with zero entries would effectively be pointless, as there
  // would be nothing there to handle interrupts, and so no pIDT_MAX_ENTRIESoint in having
  // loaded it in the first place. Since the size of 1 is useless, the length
  // field is encoded as one less than the actual length. This has the benefit
  // of reducing the 12-bit value of 4096 (for a full IDT), to a smaller 11-bit
  // value of 4096. One less bit to store!"
  //
  // https://github.com/dreamos82/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
  
  idtptr.limit = (u16) (sizeof(IdtEntry) * IDT::MAX_ENTRIES - 1);

  for (u16 i=0; i < IDT::MAX_ENTRIES; i++) {
    setEntry(i, (u64) interruptHandlerTable[i], 0x8E);
  }

  asm volatile ("lidt %0" :: "m" (idtptr));
  asm volatile ("sti");
}

auto x86_64::IDT::allocateVector() -> u8 {

  // this is the minimum idt irq
  static u16 vector = 32;

  if (vector == MAX_ENTRIES) {
    Kernel::panic("Cannot allocate interrupt vector reached maximum");
  } 

  return vector;
}

