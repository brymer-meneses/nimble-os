
#include "types.h"
#include <kernel/utils/halt.h>
#include <kernel/utils/logger.h>

// these functions need to be defined since clang expects them
extern "C" {
  // https://libcxxabi.llvm.org/spec.html

  auto __cxa_guard_acquire(u64* guard) -> int {
    return *reinterpret_cast<u8*>(guard) == 0;
  }

  auto __cxa_guard_release(u64* guard) -> void {
    *reinterpret_cast<u8*>(guard) = 0x44;
  }

  // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor-runtime-api
  void* __dso_handle;
  auto __cxa_atexit(void (*)(void*), void*, void*) -> int {
    return 0;
  }

  auto __cxa_pure_virtual(void) -> void {
    log::warn("pure virtual function invoked");
    kernel::halt();
    return;
  }
}

using ConstructorFn = void(*)();
extern ConstructorFn __init_array_start;
extern ConstructorFn __init_array_end;

namespace libcxx {
  auto callGlobalConstructors() -> void {
    for (ConstructorFn* i = &__init_array_start;i != &__init_array_end; i++) {
      (*i)();
    }
  }
}
