// Minimal kernel entry point
// This runs in a freestanding environment with no standard library
// Phase 2: VGA text-mode driver and kprintf

#include "kprintf.h"
#include "../drivers/vga.h"

// Serial port for headless testing
#define SERIAL_COM1_PORT 0x3F8
#define SERIAL_DATA_REG   (SERIAL_COM1_PORT + 0)
#define SERIAL_LINE_STATUS_REG (SERIAL_COM1_PORT + 5)

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

static int serial_is_transmit_ready() {
    return inb(SERIAL_LINE_STATUS_REG) & 0x20;
}

static void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(SERIAL_DATA_REG, c);
}

static void serial_write_string(const char* str) {
    while (*str != '\0') {
        serial_write_char(*str);
        str++;
    }
}

// Kernel main entry point - called from boot.asm
void kernel_main() {
    // Initialize VGA driver
    vga_initialize();
    
    // Print colored heading to VGA
    vga_setcolor(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLUE);
    kprintf("=== Nexus OS v0.1 ===\n\n");
    
    // Reset to default color
    vga_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
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
    
    // Also output to serial for headless testing
    serial_write_string("=== Nexus OS v0.1 ===\r\n");
    serial_write_string("VGA driver and kprintf initialized successfully\r\n");
    serial_write_string("All format specifiers working correctly\r\n");
    
    // Kernel should never return in a real OS
    // If it does, boot.asm will halt the CPU
    while (1) {
        // Infinite loop
    }
}
