#pragma once

#include <lib/types.h>
#include <limine.h>

class PhysicalAddress {
  private:
    u64 address = 0;

  public:
    constexpr PhysicalAddress(u64 address) : address(address) {} 
    constexpr PhysicalAddress() {} 

    auto toVirtual() const -> u64;

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
    constexpr VirtualAddress() {} 

    auto toPhysical() const -> u64;

    constexpr operator u64() const {
      return address;
    }  

    constexpr auto value() const -> u64 {
      return address;
    }
};
