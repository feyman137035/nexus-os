; ISR stubs for CPU exceptions
; Each stub pushes the interrupt number and jumps to common handler

section .text
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
global isr_common_stub

; External C handler
extern isr_handler

; Macro to define ISR stub
%macro ISR_NOERRCODE 1
  isr%1:
    push dword 0      ; Push dummy error code
    push dword %1     ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Macro to define ISR stub with error code
%macro ISR_ERRCODE 1
  isr%1:
    push dword %1     ; Push interrupt number (error code already pushed by CPU)
    jmp isr_common_stub
%endmacro

; Define all 32 ISR stubs
; Exceptions without error code
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8      ; Double fault (has error code)
ISR_NOERRCODE 9
ISR_ERRCODE   10     ; Invalid TSS (has error code)
ISR_ERRCODE   11     ; Segment not present (has error code)
ISR_ERRCODE   12     ; Stack fault (has error code)
ISR_ERRCODE   13     ; General protection fault (has error code)
ISR_ERRCODE   14     ; Page fault (has error code)
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17     ; Alignment check (has error code)
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; Common ISR stub
isr_common_stub:
    ; Save all registers
    pusha
    push ds
    push es
    push fs
    push gs
    
    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call C handler
    push esp
    call isr_handler
    add esp, 4
    
    ; Restore registers
    pop gs
    pop fs
    pop es
    pop ds
    popa
    
    ; Clean up error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret
