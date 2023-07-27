#include "types.h"

extern "C" {
  // https://libcxxabi.llvm.org/spec.html

  int __cxa_guard_acquire(u64* guard) {
    return *reinterpret_cast<u8*>(guard) == 0;
  }

  void __cxa_guard_release(u64* guard) {
    *reinterpret_cast<u8*>(guard) = 0x44;
  }

  // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor-runtime-api
  void* __dso_handle;
  int __cxa_atexit(void (*)(void*), void*, void*) {
    return 0;
  }
}
