#pragma once

#include <optional>
#include <tuple>
#include <lib/types.h>

namespace ACPI::MADT {

  struct LocalApic {
    u8 type;
    u8 length;
    u8 cpu;
    u8 id;
    u32 flag;
  } __attribute__((packed));

  struct IoApic {
    u8 type;
    u8 length;
    u8 id;
    u8 reserved;
    u32 addressBase;
    u32 globalInterruptBase;
  } __attribute__((packed));

  struct InterruptSourceOverride {
    u8 type;
    u8 length;
    u8 busSource;
    u8 irqSource;
    u32 globalInterrupt;
    u16 flags;
  } __attribute__((packed));

  struct NonMaskableInterrupt {
    u8 type;
    u8 length;
    u8 cpu;
    u16 flags;
    u8 lint;
  } __attribute__((packed));

  void initialize();
  auto isInitialized() -> bool;

  auto getInterruptSourceOverrides() -> std::pair<InterruptSourceOverride**, size_t>;
  auto getIoApics() -> std::pair<IoApic**, size_t>;
  auto getNonMaskableInterrupts() -> std::pair<NonMaskableInterrupt**, size_t>;
  auto getLocalApics() -> std::pair<LocalApic**, size_t>;

  auto getLocalApicAddress() -> uintptr_t;

}
