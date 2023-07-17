#include <kernel/drivers/ps2.h>
#include <kernel/drivers/apic/ioapic.h>
#include <kernel/drivers/io.h>
#include <kernel/cpu/interrupt/idt.h>
#include <kernel/cpu/interrupt/interrupt.h>
#include <kernel/utils/print.h>
#include <kernel/drivers/pic.h>

#include <limine.h>
#include <lib/types.h>

static volatile limine_smp_request smpRequest  = {
  .id = LIMINE_SMP_REQUEST,
  .revision = 0
};

void keyboardHandler() {
  u8 scancode = IO::inb(0x60);
  Kernel::println("{}", scancode);

  PIC::sendEndOfInterrupt(33);
}

// static void write(u16 port, u8 value) {
//   while ((IO::Port::inb(0x64) & 2) != 0);
//   IO::Port::out(port, value);
// }

void PS2::Keyboard::initialize() {

  // limine_smp_response* smpResponse = smpRequest.response;
  // u32 bspLapicId = smpResponse->bsp_lapic_id;

  // write(0x64, 0xae);

  PIC::initialize();
  PIC::clearMask(1);

  // IOAPIC::setRedirect(bspLapicId, vector, 1, true);
  Interrupt::setIrqHandler(33, keyboardHandler);
}
