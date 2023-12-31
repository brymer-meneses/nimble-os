#include <kernel/utils/print.h>
#include <kernel/utils/logger.h>

#include "gdt.h"
#include <array>

struct GdtEntry {
  u16 limit_low;
  u16 base_low;
  u8 base_middle;
  u8 access;
  u8 flags;
  u8 base_high;
} __attribute__((packed));

struct GdtPtr {
  u16 limit;
  u64 base;
} __attribute__((packed));

static constexpr u64 TOTAL_ENTRIES = 5;

static std::array<GdtEntry, TOTAL_ENTRIES> gdt;

auto setEntry(size_t index, u8 access, u8 flags) -> void {
  gdt[index].base_low = 0x0000;
  gdt[index].base_middle = 0x00;
  gdt[index].base_high = 0x00;
  gdt[index].limit_low = 0x0000;
  gdt[index].access = access;
  gdt[index].flags = flags;
}

// defined in `gdt.asm`
extern "C" void loadGDT(GdtPtr* gdt);

auto x86_64::GDT::initialize() -> void {
  GdtPtr gdtptr;
  gdtptr.limit = sizeof(GdtEntry) * TOTAL_ENTRIES - 1;
  gdtptr.base = (u64) &gdt;

  setEntry(0, 0, 0);
  setEntry(1, 0x9A, 0xA0);
  setEntry(2, 0x92, 0xC0);
  setEntry(3, 0xFA, 0xA0);
  setEntry(4, 0xF2, 0xC0);

  loadGDT(&gdtptr);
  log::info("Initialized GDT at {#0x16}", &gdt);
}

