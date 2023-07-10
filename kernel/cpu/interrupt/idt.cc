#include "idt.h"
#include "lib/kernel/print.h"
#include <stdint.h>

// NOTE:
// I once can't get interrupt working because `isr_high` is set to `16` bits
// Qwinci from OSDev helped me sort it out.
struct IdtEntry {
  uint16_t isr_low;
  uint16_t kernel_code_segment;
  uint8_t ist;
  uint8_t attributes;
  uint16_t isr_mid;
  uint32_t isr_high;
  uint32_t reserved;
} __attribute__((packed));

struct IdtPtr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

static constexpr uint16_t KERNEL_CODE_SEGMENT = 0x08;

__attribute__((aligned(0x10))) 
static IdtEntry idt[IDT::MAX_ENTRIES];

static void setEntry(uint8_t vector, uint64_t handler, uint8_t flags) {

  idt[vector].isr_low = handler & 0xFFFF;
  idt[vector].isr_mid = (handler >> 16) & 0xFFFF;
  idt[vector].isr_high = (handler >> 32) & 0xFFFFFFFF;

  idt[vector].kernel_code_segment = KERNEL_CODE_SEGMENT;
  idt[vector].ist = 0;
  idt[vector].attributes = flags;
  idt[vector].reserved = 0;
}

extern void* interruptHandlerTable[];

void IDT::initialize() {
  IdtPtr idtptr;

  idtptr.base = (uint64_t) &idt[0];

  // "The reason for subtracting one from the size of the idt is interesting.
  // Loading an IDT with zero entries would effectively be pointless, as there
  // would be nothing there to handle interrupts, and so no pIDT_MAX_ENTRIESoint in having
  // loaded it in the first place. Since the size of 1 is useless, the length
  // field is encoded as one less than the actual length. This has the benefit
  // of reducing the 12-bit value of 4096 (for a full IDT), to a smaller 11-bit
  // value of 4096. One less bit to store!"
  //
  // https://github.com/dreamos82/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
  
  idtptr.limit = (uint16_t) (sizeof(IdtEntry) * IDT::MAX_ENTRIES - 1);

  for (uint16_t i=0; i < 32; i++) {
    setEntry(i, (uint64_t) interruptHandlerTable[i], 0x8E);
  }

  asm volatile ("lidt %0" :: "m" (idtptr));
  asm volatile ("sti");
}
