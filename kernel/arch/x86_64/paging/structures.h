#pragma once

#include <lib/types.h>
#include <lib/bit.h>

#include <kernel/utils/assert.h>
#include <kernel/memory/vmm.h>

namespace x86_64 {

  constexpr u64 PML_PRESENT = 1;
  constexpr u64 PML_WRITEABLE = 1;
  constexpr u64 PML_USER_ACCESSIBLE = 1;
  constexpr u64 PML_WRITE_THROUGH = 1;
  constexpr u64 PML_CACHE_DISABLED = 1;
  constexpr u64 PML_ACCESSED = 1;
  constexpr u64 PML_NOT_EXECUTABLE = 1ULL << 63;

  struct PageMapLevelEntry {
    private:
      u64 data = 0;

    public:
      constexpr auto address() const -> PhysicalAddress {
        auto addressMasked = Bit::mask(data, 63-39, 11);
        return addressMasked >> 12;
      }
      constexpr auto setAddress(const PhysicalAddress& address) -> void {
        data = ~Bit::mask(data, 63-39, 9) | (address << 11);
      }

      constexpr auto available() const -> u8 {
        auto availableMasked = Bit::mask(data, 39, 11);
        return availableMasked >> 9;
      }
      constexpr auto setAvailable(u8 value) -> void {
        // ensure only 4 bits are used by the value
        Kernel::assert((value & u64(0b1111)) == 0);

        data &= ~Bit::mask(data, 63-39, 9) | (value << 11);
      }

      constexpr auto setFlags(u64 flags) {
        data |= flags;
      }

      constexpr auto testFlag(u64 flag) -> bool {
        return (data & ~flag) != 0;
      }
  };

  constexpr u64 PTE_PRESENT = 1;
  constexpr u64 PTE_WRITEABLE = 1;
  constexpr u64 PTE_USER_ACCESSIBLE = 1;
  constexpr u64 PTE_WRITE_THROUGH = 1;
  constexpr u64 PTE_CACHE_DISABLED = 1;
  constexpr u64 PTE_ACCESSED = 1;
  constexpr u64 PTE_DIRTY = 1;
  constexpr u64 PTE_PS = 1;
  constexpr u64 PTE_PAT = 1;
  constexpr u64 PTE_GLOBAL = 1;
  constexpr u64 PTE_NOT_EXECUTABLE = 1ULL << 63;

  struct PageTableEntry {
    private:
      u64 data = 0;

    public:
      constexpr auto address() const -> PhysicalAddress {
        auto addressMasked = Bit::mask(data, 63-39, 11);
        return addressMasked >> 12;
      }
      constexpr auto setAddress(const PhysicalAddress& address) -> void {
        data = ~Bit::mask(data, 63-39, 9) | (address << 11);
      }

      constexpr auto setAvailable(u8 value) -> void {
        // ensure only 4 bits are used by the value
        Kernel::assert((value & u64(0b1111)) == 0);
        data = ~Bit::mask(data, 63-39, 9) | (value << 11);
      }

      constexpr auto setFlags(u64 vmmflags) {
        data |= vmmflags;
      }
  };

}
