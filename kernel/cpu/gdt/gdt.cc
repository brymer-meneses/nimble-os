#include "gdt.h"

struct GdtEntry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t flags;
  uint8_t base_high;
} __attribute__((packed));

struct GdtPtr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

static constexpr uint64_t TOTAL_ENTRIES = 5;

static GdtEntry gdt[TOTAL_ENTRIES];

void setEntry(size_t i, uint8_t access, uint8_t flags) {

  gdt[i].base_low    = 0x0000;
  gdt[i].base_middle = 0x00;
  gdt[i].base_high   = 0x00;
  gdt[i].access      = access;
  gdt[i].limit_low   = 0x0000;
  gdt[i].flags       = flags;

}

extern "C" void loadGDT(GdtPtr* gdtptr);

void GDT::initialize() {

  GdtPtr gdtptr;
  gdtptr.limit = sizeof(GdtEntry) * TOTAL_ENTRIES - 1;
  gdtptr.base = (uint64_t) &gdt;

  // https://wiki.osdev.org/GDT_Tutorial
  // Because `sizeof(GdtEntry) == 8`, the offset in the table
  // refers to this
  setEntry(0,    0,   0);   // Null descriptor
  setEntry(1, 0x9A, 0xA0);   // Kernel Mode Code Segment
  setEntry(2, 0x92, 0xC0);   // Kernel Mode Data Segment
  setEntry(3, 0xFA, 0xA0);   // User Mode Code Segment
  setEntry(4, 0xF2, 0xC0);   // User Mode Data Segment

  loadGDT(&gdtptr);
}

