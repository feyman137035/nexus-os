; IRQ stubs for hardware interrupts
; Each stub pushes the interrupt number and jumps to common handler

section .text
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
global irq_common_stub

; External C handler
extern irq_handler

; Macro to define IRQ stub
%macro IRQ_STUB 1
  irq%1:
    push dword 0      ; Push dummy error code (IRQs don't push error codes)
    push dword (%1 + 32) ; Push interrupt number (IRQ number + 32)
    jmp irq_common_stub
%endmacro

; Define all 16 IRQ stubs
IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15

; Common IRQ stub
irq_common_stub:
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
    call irq_handler
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
