
global loadGDT
bits 64

loadGDT:
  lgdt [rdi]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  pop rdi
  push 0x08
  push rdi
  retfq