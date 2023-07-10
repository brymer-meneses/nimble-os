[bits 64]
; NOTE:
; I would really like to use `snake_case` naming here but
; I would rather maintain consistency

; macro for defining an interrupt handler that returns 
; an error code
%macro setErrorInterruptHandler 1
global interruptHandler%1
interruptHandler%1:
  push qword %1                   ; push the interrupt number
  jmp commonInterruptHandler
%endmacro

; macro for defining an interrupt handler that DOES not
; return an error code

%macro setNoErrorInterruptHandler 1
global interruptHandler%1
interruptHandler%1:
  push qword 0                    ; push 0 error code
  push qword %1                   ; push the interrupt number
  jmp commonInterruptHandler
%endmacro

; this is defined on `interrupt.cc`
extern interruptDispatch

commonInterruptHandler:
  push rax
  push rbx
  push rcx
  push rdx
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov rdi, rsp
  call interruptDispatch

  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rdx
  pop rcx
  pop rbx
  pop rax

  add rsp, 16
  iretq


setNoErrorInterruptHandler 0
setNoErrorInterruptHandler 1
setNoErrorInterruptHandler 2
setNoErrorInterruptHandler 3
setNoErrorInterruptHandler 4
setNoErrorInterruptHandler 5
setNoErrorInterruptHandler 6
setNoErrorInterruptHandler 7
setErrorInterruptHandler 8
setNoErrorInterruptHandler 9
setErrorInterruptHandler 10
setErrorInterruptHandler 11
setErrorInterruptHandler 12
setErrorInterruptHandler 13
setErrorInterruptHandler 14
setNoErrorInterruptHandler 15
setNoErrorInterruptHandler 16
setErrorInterruptHandler 17
setNoErrorInterruptHandler 18
setNoErrorInterruptHandler 19
setNoErrorInterruptHandler 20
setNoErrorInterruptHandler 21
setNoErrorInterruptHandler 22
setNoErrorInterruptHandler 23
setNoErrorInterruptHandler 24
setNoErrorInterruptHandler 25
setNoErrorInterruptHandler 26
setNoErrorInterruptHandler 27
setNoErrorInterruptHandler 28
setNoErrorInterruptHandler 29
setErrorInterruptHandler 30
setNoErrorInterruptHandler 31

global interruptHandlerTable
interruptHandlerTable:
%assign i 0
%rep 32
  dq interruptHandler%+i
%assign i i+1
%endrep
  
