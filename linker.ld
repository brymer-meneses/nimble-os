ENTRY(start)

SECTIONS {
  . = 1M;

  .boot : 
  {
    KEEP(*(.multiboot_header))
  }

  .text : 
  {
    *(.text)
  }

  /* Include the list of initialization functions sorted. */
  .init_array :
  {
      *crti.asm.o(.init_array)
      KEEP (*(SORT(EXCLUDE_FILE(*crti.asm.o *crtn.asm.o) .init_array.*)))
      KEEP (*(EXCLUDE_FILE(*crti.asm.o *crtn.asm.o) .init_array))
      *crtn.asm.o(.init_array)
  }

  /* Include the list of termination functions sorted. */
  .fini_array :
  {
      *crti.asm.o(.fini_array)
      KEEP (*(SORT(EXCLUDE_FILE(*crti.asm.o *crtn.asm.o) .fini_array.*)))
      KEEP (*(EXCLUDE_FILE(*crti.asm.o *crtn.asm.o) .fini_array))
      *crtn.asm.o(.fini_array)
  }
}
