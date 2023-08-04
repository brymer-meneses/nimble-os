#include <kernel/utils/panic.h>
#include <kernel/utils/print.h>

#include <limine.h>

#include "memory.h"
#include "pmm.h"

auto Memory::initialize() -> void {
  PMM::initialize();
  VMM::initialize();
}

