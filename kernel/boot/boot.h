#pragma once

#include <limine.h>

namespace boot {
  extern limine_hhdm_request hhdmRequest;
  extern limine_framebuffer_request framebufferRequest;
  extern limine_memmap_request memmapRequest;
  extern limine_rsdp_request rsdpRequest;
  extern limine_kernel_address_request kernelAddressRequest;
}
