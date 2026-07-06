// Minimal kernel entry point
// This runs in a freestanding environment with no standard library
// Phase 4: GDT, IDT, ISR, IRQ, timer, keyboard, and shell

#include "kprintf.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "shell.h"
#include "../drivers/vga.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"

// Kernel main entry point - called from boot.asm
void kernel_main() {
    // Initialize VGA driver
    vga_initialize();
    
    // Print colored heading to VGA
    vga_setcolor(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    kprintf("=== Nexus OS v0.2 ===\n\n");
    
    // Reset to default color
    vga_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Initialize GDT
    kprintf("Initializing GDT...\n");
    gdt_install();
    
    // Initialize IDT
    kprintf("Initializing IDT...\n");
    idt_install();
    
    // Install ISRs (CPU exception handlers)
    kprintf("Installing ISRs...\n");
    isr_install();
    
    // Install IRQs (hardware interrupt handlers)
    kprintf("Installing IRQs...\n");
    irq_install();
    
    // Initialize timer (100Hz)
    kprintf("Initializing timer...\n");
    timer_init(100);
    
    // Enable interrupts
    kprintf("Enabling interrupts...\n");
    __asm__ volatile("sti");
    
    // Initialize keyboard driver
    kprintf("Initializing keyboard...\n");
    keyboard_init();
    
    kprintf("\nSystem initialization complete!\n\n");
    
    // Start the interactive shell
    shell_run();
}
