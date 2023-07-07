#pragma once

#ifndef ARCH
  #error "ARCH is not defined. Please specify the arch you are compiling to."
#endif

#if ARCH == x86_64
  #include "x86_64/base.h"
#endif
