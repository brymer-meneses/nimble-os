
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

namespace lib {

  void format(char* buffer, const char* string, ...);

  namespace internal {
    void format_impl(char* buffer, const char* string, va_list args);
  }

}
