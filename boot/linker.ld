/* Linker script for Nexus OS kernel
 * This defines how the kernel is laid out in memory
 */

/* Entry point - must match the label in boot.asm */
ENTRY(_start)

/* Kernel load address: 1MB (0x100000)
 * This is the standard higher-half-free load address for Multiboot kernels
 * It avoids the lower 640KB reserved for BIOS and legacy hardware
 */
SECTIONS
{
    /* Start of kernel at 1MB */
    . = 0x100000;

    /* Text section - contains executable code
     * This includes boot.asm and kernel.c compiled code
     */
    .text :
    {
        *(.multiboot)  /* Multiboot header must be first */
        *(.text)        /* All other code */
    }

    /* Read-only data section - contains constants and strings
     * This includes string literals like "Hello from Nexus OS"
     */
    .rodata :
    {
        *(.rodata)
    }

    /* Read-write data section - contains initialized global/static variables
     * Variables with initial values go here
     */
    .data :
    {
        *(.data)
    }

    /* BSS section - contains uninitialized global/static variables
     * This section is zeroed by the bootloader at startup
     * Includes our stack
     */
    .bss :
    {
        *(.COMMON)
        *(.bss)
    }

    /* Discard sections we don't need */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
