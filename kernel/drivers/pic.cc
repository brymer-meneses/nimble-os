#include "pic.h"
#include <kernel/drivers/io.h>
#include <kernel/utils/print.h>
#include <lib/types.h>

#define ICW1_ICW4           0x01
#define ICW1_SINGLE         0x02
#define ICW1_INTERVAL4      0x04
#define ICW1_LEVEL          0x08
#define ICW1_INIT           0x10

#define ICW4_8086           0x01
#define ICW4_AUTO           0x02
#define ICW4_BUF_SLAVE      0x08
#define ICW4_BUF_MASTER     0x0C
#define ICW4_SFNM           0x10

#define PIC1		            0x20		
#define PIC2		            0xA0		
#define PIC1_COMMAND	      PIC1
#define PIC1_DATA	          (PIC1+1)
#define PIC2_COMMAND	      PIC2
#define PIC2_DATA	          (PIC2+1)

#define PIC1_OFFSET         0x20
#define PIC2_OFFSET         0x28
#define PIC_EOI             0x20

void PIC::initialize() {
  
uint8_t a1, a2;

  // sanity checks
  static_assert(PIC1_OFFSET % 8 == 0);
  static_assert(PIC2_OFFSET % 8 == 0);
  // make sure they don't overlap
  static_assert(PIC2_OFFSET - PIC1_OFFSET >= 8);

  // save masks
  a1 = IO::inb(PIC1_DATA);
  a2 = IO::inb(PIC2_DATA);

  // start the initialization sequence (in cascade mode)
  IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  IO::wait();
  IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  IO::wait();

  // Port the master PIC vector offset
  IO::outb(PIC1_DATA, PIC1_OFFSET);
  IO::wait();

  // Set the slave PIC vector offset
  IO::outb(PIC2_DATA, PIC2_OFFSET);
  IO::wait();

  // Tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
  IO::outb(PIC1_DATA, 4);
  IO::wait();

  // Tell slave PIC its cascade identity (0000 0010)
  IO::outb(PIC2_DATA, 2);
  IO::wait();

  IO::outb(PIC1_DATA, ICW4_8086);
  IO::wait();
  IO::outb(PIC2_DATA, ICW4_8086);
  IO::wait();

  // :Port::estore saved masks
  IO::outb(PIC1_DATA, a1);
  IO::outb(PIC2_DATA, a2);
}

void PIC::maskAll() {
  IO::outb(PIC1_DATA, 0xff);
  IO::outb(PIC1_DATA, 0xff);
}

void PIC::sendEndOfInterrupt(u8 irq) {
  if (irq >= 8) {
    IO::outb(PIC2_COMMAND, PIC_EOI);
  };

  IO::outb(PIC1_COMMAND, PIC_EOI);
}

void PIC::clearMask(u8 irq) {
  u16 port;
  u8 value;

  if (irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC1_DATA;
    irq -= 8;
  }

  value = IO::inb(port) & ~(1 << irq);
  IO::outb(port, value);
}
