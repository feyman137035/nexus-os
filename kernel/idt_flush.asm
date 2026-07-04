; IDT flush assembly routine
; This loads the IDT

section .text
global idt_flush

; idt_flush(uint32_t idt_ptr_address)
; Input: EAX = address of IDT pointer structure
idt_flush:
    ; Load IDT pointer from stack
    mov eax, [esp + 4]
    lidt [eax]
    ret
