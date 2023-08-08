#pragma once

#include <lib/types.h>
#include <limine.h>

class VirtualAddress;

class PhysicalAddress {
  private:
    u64 address = 0;

  public:
    constexpr PhysicalAddress(u64 address) : address(address) {} 
    constexpr PhysicalAddress() = default;

    auto toVirtual() const -> VirtualAddress;
    auto ptr() -> void*;

    constexpr operator u64() const {
      return address;
    }  

    constexpr auto value() const -> u64 {
      return address;
    }
};

class VirtualAddress {
  private:
    u64 address = 0;

  public:
    constexpr VirtualAddress(u64 address) : address(address) {} 
    constexpr VirtualAddress() = default;

    auto toPhysical() const -> PhysicalAddress;
    auto ptr() -> void*;

    constexpr operator u64() const {
      return address;
    }  

    constexpr auto value() const -> u64 {
      return address;
    }
};
