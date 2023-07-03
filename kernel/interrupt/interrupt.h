#pragma once

#include <stdint.h>

namespace interrupt {

void initialize_idt();
void set_descriptor(uint8_t vector, uint64_t handler, uint8_t dpl);

}
