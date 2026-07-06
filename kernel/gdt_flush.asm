; GDT flush assembly routine
; This loads the GDT and reloads all segment registers

section .text
global gdt_flush

; gdt_flush(uint32_t gdt_ptr_address)
; Input: EAX = address of GDT pointer structure
gdt_flush:
    ; Load GDT pointer from stack
    mov eax, [esp + 4]
    lgdt [eax]
    
    ; Reload segment registers
    ; CS cannot be loaded directly, must use far jump
    mov ax, 0x10      ; Kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Reload CS using far jump
    jmp 0x08:.continue
.continue:
    ; Return to caller (normal return)
    ret
