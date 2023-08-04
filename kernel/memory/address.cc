#include "address.h"
#include "vmm.h"


auto PhysicalAddress::toVirtual() const -> u64 {
  return VMM::physicalToVirtual(address);
}

auto VirtualAddress::toPhysical() const -> u64 {
  return VMM::virtualToPhysical(address);
}
