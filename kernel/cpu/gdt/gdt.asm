
; NOTE:
; the placement in the `gdt` affects these values
%define KERNEL_DATA_SEGMENT 0x10
%define KERNEL_CODE_SEGMENT 0x08

global loadGDT
bits 64

loadGDT:
  lgdt [rdi]
  mov ax, KERNEL_DATA_SEGMENT
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  pop rdi
  push KERNEL_CODE_SEGMENT
  push rdi
  retfq
