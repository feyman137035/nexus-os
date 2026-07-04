#include "timer.h"
#include "../kernel/irq.h"
#include "../kernel/kprintf.h"

// Tick counter
static uint32_t timer_ticks = 0;

// I/O port functions
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

// Timer interrupt handler
static void timer_handler(registers_t *regs) {
    timer_ticks++;
}

// Initialize timer
void timer_init(uint32_t frequency) {
    // Calculate divisor
    uint32_t divisor = 1193180 / frequency;
    
    // Send command to PIT (channel 0, low+high byte, square wave, binary)
    outb(0x43, 0x36);
    
    // Send divisor (low byte then high byte)
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
    
    // Install IRQ handler
    irq_install_handler(0, timer_handler);
    
    kprintf("Timer initialized at %d Hz\n", frequency);
}

// Get current tick count
uint32_t timer_get_ticks(void) {
    return timer_ticks;
}
