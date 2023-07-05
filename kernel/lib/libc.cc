#include "libc.h"
#include <stdarg.h>
#include <stddef.h>

namespace std {

  size_t strlen(const char* string)  {
    size_t i = 0;
    while (string[i] != '\0') {
      i++;
    }  
    return i;
  }
  
  // https://android.googlesource.com/platform/bionic/+/a27d2baa/libc/string/strcmp.c
  int strcmp(const char* s1, const char* s2) {
    while (*s1 == *s2++)
		  if (*s1++ == 0)
			  return (0);
    
	    return (*(unsigned char *)s1 - *(unsigned char *)--s2);
  };

}
