#include <stddef.h>
#include "libc.h"
#include <kernel/utils/panic.h>

extern "C" {

void abort() {
  Kernel::panic("LIBC Abort");
};

void* std::memcpy(void *dest, const void *src, size_t n) {
    u8 *pdest = (u8 *)dest;
    const u8 *psrc = (const u8 *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void* std::memset(void *s, int c, size_t n) {
    u8 *p = (u8 *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (u8)c;
    }

    return s;
}

void* std::memmove(void *dest, const void *src, size_t n) {
    u8 *pdest = (u8 *)dest;
    const u8 *psrc = (const u8 *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int std::memcmp(const void *s1, const void *s2, size_t n) {
    const u8 *p1 = (const u8 *)s1;
    const u8 *p2 = (const u8 *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
} 

int std::strcmp(const char* s1, const char* s2) {
  while (*s1 == *s2++)
    if (*s1++ == '\0')
        return (0);
  return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
}

}
