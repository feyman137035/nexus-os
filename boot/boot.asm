; Multiboot-compliant bootloader entry point
; This file is loaded by GRUB and sets up the environment before calling kernel_main

; Multiboot header - Required for GRUB to recognize this as a valid kernel
; The header must be within the first 8192 bytes of the kernel image and aligned on 4-byte boundary
section .multiboot
align 4
multiboot_header_start:
    ; Magic number - identifies this as a Multiboot-compliant kernel
    ; GRUB checks for this specific value: 0x1BADB002
    dd 0x1BADB002
    
    ; Flags - tells GRUB which features we require
    ; 0x00000000 = no special requirements (we don't need memory info, etc.)
    dd 0
    
    ; Checksum - ensures header integrity
    ; Must be: -(magic + flags) modulo 2^32
    ; -(0x1BADB002 + 0) = -0x1BADB002 = 0xE4524FFE
    dd -(0x1BADB002 + 0)
multiboot_header_end:

; Kernel entry point - this is where execution begins after GRUB loads us
section .text
global _start
extern kernel_main

_start:
    ; Set up stack - we need a stack for function calls
    ; Place stack at the end of lower memory (safe area)
    ; 0x800000 is 8MB, we grow downward from there
    mov esp, stack_top
    
    ; Save the Multiboot information structure pointer
    ; GRUB passes this in EBX, we might need it later for memory info
    push ebx
    
    ; Call our C kernel entry point
    call kernel_main
    
    ; If kernel_main returns, halt the CPU
    ; In a real OS, kernel_main should never return
    cli                 ; Disable interrupts
.halt_loop:
    hlt                 ; Halt the CPU until next interrupt
    jmp .halt_loop      ; Infinite loop (should never reach here)

; Stack section - reserves space for the stack
section .bss
align 16
stack_bottom:
    ; Reserve 16KB for stack (4096 * 4 bytes)
    resb 16384
stack_top:
