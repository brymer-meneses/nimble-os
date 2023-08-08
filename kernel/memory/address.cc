#include "address.h"
#include "vmm.h"


auto PhysicalAddress::toVirtual() const -> VirtualAddress {
  return VMM::physicalToVirtual(address);
}

auto PhysicalAddress::ptr() -> void* {
  return reinterpret_cast<void*>(address);
}

auto VirtualAddress::toPhysical() const -> PhysicalAddress {
  return VMM::virtualToPhysical(address);
}
auto VirtualAddress::ptr() -> void* {
  return reinterpret_cast<void*>(address);
}


