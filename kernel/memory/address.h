#pragma once

#include <lib/types.h>
#include <limine.h>

class PhysicalAddress {
  private:
    u64 address = 0;

  public:
    PhysicalAddress(u64 address) : address(address) {} 
    PhysicalAddress() {} 

    auto toVirtual() const -> u64;

    constexpr operator u64() {
      return address;
    }  
};

class VirtualAddress {
  private:
    u64 address = 0;

  public:
    VirtualAddress(u64 address) : address(address) {} 
    VirtualAddress() {} 

    auto toPhysical() const -> u64;

    constexpr operator u64() {
      return address;
    }  
};
