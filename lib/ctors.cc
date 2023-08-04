#include "ctors.h"

auto callGlobalConstructors() -> void {
  using ConstructorFn = void(*)();
  extern ConstructorFn start_ctors;
  extern ConstructorFn end_ctors;

  for(ConstructorFn* i = &start_ctors;i != &end_ctors; i++) {
    (*i)();
  }
}

