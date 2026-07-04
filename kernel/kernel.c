// Minimal kernel entry point
// This runs in a freestanding environment with no standard library
// Phase 3: GDT, IDT, ISR, IRQ, and timer interrupt handling

#include "kprintf.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "../drivers/vga.h"
#include "../drivers/timer.h"

// Minimal serial output for debugging
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static void serial_write_char(char c) {
    while (!(inb(0x3F8 + 5) & 0x20));
    outb(0x3F8, c);
}

static void serial_write_string(const char* str) {
    while (*str) {
        serial_write_char(*str++);
    }
}

// Kernel main entry point - called from boot.asm
void kernel_main() {
    serial_write_string("kernel_main: entry\r\n");
    
    // Initialize VGA driver
    vga_initialize();
    serial_write_string("kernel_main: VGA initialized\r\n");
    
    // Print colored heading to VGA
    vga_setcolor(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    kprintf("=== Nexus OS v0.2 ===\n\n");
    
    // Reset to default color
    vga_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Initialize GDT
    kprintf("Initializing GDT...\n");
    serial_write_string("kernel_main: calling gdt_install\r\n");
    gdt_install();
    serial_write_string("kernel_main: gdt_install returned\r\n");
    
    // Initialize IDT
    kprintf("Initializing IDT...\n");
    serial_write_string("kernel_main: calling idt_install\r\n");
    idt_install();
    serial_write_string("kernel_main: idt_install returned\r\n");
    
    // Install ISRs (CPU exception handlers)
    kprintf("Installing ISRs...\n");
    serial_write_string("kernel_main: calling isr_install\r\n");
    isr_install();
    serial_write_string("kernel_main: isr_install returned\r\n");
    
    // Install IRQs (hardware interrupt handlers)
    kprintf("Installing IRQs...\n");
    serial_write_string("kernel_main: calling irq_install\r\n");
    irq_install();
    serial_write_string("kernel_main: irq_install returned\r\n");
    
    // Initialize timer (100Hz)
    kprintf("Initializing timer...\n");
    serial_write_string("kernel_main: calling timer_init\r\n");
    timer_init(100);
    serial_write_string("kernel_main: timer_init returned\r\n");
    
    // Enable interrupts
    kprintf("Enabling interrupts...\n");
    serial_write_string("kernel_main: enabling interrupts\r\n");
    __asm__ volatile("sti");
    serial_write_string("kernel_main: interrupts enabled\r\n");
    
    kprintf("\nSystem initialization complete!\n\n");
    serial_write_string("kernel_main: init complete, entering loop\r\n");
    
    // Demonstrate kprintf with various format specifiers
    kprintf("Testing kprintf() functionality:\n\n");
    
    // String printing
    kprintf("String: %s\n", "Hello from Nexus OS");
    
    // Character printing
    kprintf("Character: %c\n", 'X');
    
    // Decimal numbers
    kprintf("Decimal: %d\n", 42);
    kprintf("Negative: %d\n", -123);
    
    // Unsigned numbers
    kprintf("Unsigned: %u\n", 255);
    
    // Hexadecimal
    kprintf("Hexadecimal: %x\n", 0xDEADBEEF);
    
    // Literal percent
    kprintf("Literal percent: %%\n\n");
    
    // Demonstrate a small table
    vga_setcolor(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    kprintf("Number Table:\n");
    vga_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    kprintf("Decimal | Hex\n");
    kprintf("--------|----\n");
    kprintf("%8d|%4x\n", 0, 0);
    kprintf("%8d|%4x\n", 10, 10);
    kprintf("%8d|%4x\n", 255, 255);
    kprintf("%8d|%4x\n", 4096, 4096);
    kprintf("%8d|%4x\n", 65535, 65535);
    
    kprintf("\nTimer ticks: ");
    uint32_t last_ticks = 0;
    
    // Kernel should never return in a real OS
    // If it does, boot.asm will halt the CPU
    while (1) {
        // Print timer ticks periodically
        uint32_t current_ticks = timer_get_ticks();
        if (current_ticks != last_ticks && current_ticks % 100 == 0) {
            kprintf("%d ", current_ticks);
            
            // Format and write to serial
            char buf[16];
            int temp = current_ticks;
            int idx = 0;
            char temp_buf[16];
            int t_idx = 0;
            while (temp > 0) {
                temp_buf[t_idx++] = '0' + (temp % 10);
                temp = temp / 10;
            }
            while (t_idx > 0) {
                buf[idx++] = temp_buf[--t_idx];
            }
            buf[idx++] = ' ';
            buf[idx++] = '\0';
            serial_write_string(buf);
            
            last_ticks = current_ticks;
        }
        
        // Small delay to prevent flooding output
        for (volatile int i = 0; i < 100000; i++);
    }
}
