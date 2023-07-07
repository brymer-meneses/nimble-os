#include "pic.h"
#include "io.h"
#include "serial.h"

constexpr uint16_t ICW1_ICW4         = 0x01;
constexpr uint16_t ICW1_SINGLE       = 0x02;
constexpr uint16_t ICW1_INTERVAL4    = 0x04;
constexpr uint16_t ICW1_LEVEL        = 0x08;
constexpr uint16_t ICW1_INIT         = 0x10;

constexpr uint16_t ICW4_INIT         = 0x10;
constexpr uint16_t ICW4_8086         = 0x01;
constexpr uint16_t ICW4_AUTO         = 0x02;
constexpr uint16_t ICW4_BUF_SLAVE    = 0x08;
constexpr uint16_t ICW4_BUF_MASTER   = 0x0C;
constexpr uint16_t ICW4_SFNM         = 0x10;

constexpr uint16_t PIC1              = 0x20;
constexpr uint16_t PIC2              = 0xA0;

constexpr uint16_t PIC1_COMMAND      = PIC1;
constexpr uint16_t PIC1_DATA         = PIC1 + 1;
constexpr uint16_t PIC2_COMMAND      = PIC2;
constexpr uint16_t PIC2_DATA         = PIC2 + 1;

constexpr uint16_t PIC_EOI           = 0x20;

constexpr uint16_t PIC_MASTER_OFFSET = 0x20;
constexpr uint16_t PIC_SLAVE_OFFSET  = 0x28;

void pic::initialize() {
  uint8_t a1, a2;

  // save masks
  a1 = io::inb(PIC1_DATA);
  a2 = io::inb(PIC2_DATA);

  // start the initialization sequence (in cascade mode)
  io::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  io::wait();
  io::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io::wait();

  // set the master PIC vector offset
  io::outb(PIC1_DATA, PIC_MASTER_OFFSET);
  io::wait();

  // set the slave PIC vector offset
  io::outb(PIC2_DATA, PIC_SLAVE_OFFSET);
  io::wait();

  // tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
  io::outb(PIC1_DATA, 4);
  io::wait();

  // tell slave PIC its cascade identity (0000 0010)
  io::outb(PIC2_DATA, 2);
  io::wait();

  io::outb(PIC1_DATA, ICW4_8086);
  io::wait();
  io::outb(PIC2_DATA, ICW4_8086);
  io::wait();

  // restore saved masks
  io::outb(PIC1_DATA, a1);
  io::outb(PIC2_DATA, a2);

  for (int i=0; i<16; i++) {
    pic::clear_mask(i);
  }
}

void pic::set_mask(uint8_t irqline) {
  uint16_t port;
  uint8_t value;

  if (irqline < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irqline -= 8;
  }
  value = io::inb(port) | (1 << irqline);
  io::outb(port, value);
}

void pic::clear_mask(uint8_t irqline) {
  uint16_t port;
  uint8_t value;

  if (irqline < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irqline -= 8;
  }
  value = io::inb(port) & ~(1 << irqline);
  io::outb(port, value);
}

void pic::finish_interrupt(uint8_t irq) {
  if (irq >= 8) {
    io::outb(PIC2_COMMAND, PIC_EOI);
  }

  io::outb(PIC1_COMMAND, PIC_EOI);
}

void pic::disable() {
  asm volatile(
    "mov al, 0xff\n"
    "out 0xa1, al\n"
    "out 0x21, al\n"
  );
}
