#include "gdt.h"

#include <lib/kernel/print.h>

struct GdtEntry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

struct GdtPtr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

static constexpr uint64_t TOTAL_ENTRIES = 5;

static GdtEntry gdt[TOTAL_ENTRIES];

void setEntry(size_t index, uint8_t access, uint8_t granularity) {
  gdt[index].base_low = 0x0000;
  gdt[index].base_middle = 0x00;
  gdt[index].base_high = 0x00;
  gdt[index].limit_low = 0x0000;
  gdt[index].access = access;
  gdt[index].granularity = granularity;
}

extern "C" void loadGDT(GdtPtr* gdt);

void GDT::initialize() {
  GdtPtr gdtptr;
  gdtptr.limit = sizeof(GdtEntry) * TOTAL_ENTRIES - 1;
  gdtptr.base = (uint64_t) &gdt;

  Kernel::print("Initializing GDT ... ");

  setEntry(0, 0, 0);
  setEntry(1, 0x9A, 0xA0);
  setEntry(2, 0x92, 0xC0);
  setEntry(3, 0xFA, 0xA0);
  setEntry(4, 0xF2, 0xC0);

  loadGDT(&gdtptr);

  Kernel::println("[done]");
}

