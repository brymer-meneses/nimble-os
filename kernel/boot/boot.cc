#include <limine.h>

namespace boot {

  volatile auto hhdmRequest = limine_hhdm_request {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
  };

  volatile auto framebufferRequest = limine_framebuffer_request {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
  };

  volatile auto memmapRequest = limine_memmap_request {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
  };
  
  volatile auto rsdpRequest = limine_rsdp_request {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
  };

};
