#include "irq.h"
#include "idt.h"
#include "kprintf.h"
#include <stdint.h>

// IRQ handler array (16 IRQs)
static irq_handler_t irq_handlers[16];

// External assembly IRQ handler
extern void irq_common_stub(void);

// I/O port functions
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

// Remap PIC
void pic_remap(void) {
    // Save masks
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);
    
    // Start initialization sequence
    outb(0x20, 0x11);  // ICW1: init, ICW4 needed
    outb(0xA0, 0x11);
    
    // Set vector offsets (IRQ0-7 → INT32-39, IRQ8-15 → INT40-47)
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    
    // Configure cascade
    outb(0x21, 0x04);  // ICW3: master PIC, IRQ2 has slave
    outb(0xA1, 0x02);  // ICW3: slave PIC, cascade identity
    
    // Set 8086 mode
    outb(0x21, 0x01);  // ICW4: 8086 mode
    outb(0xA1, 0x01);
    
    // Restore masks
    outb(0x21, a1);
    outb(0xA1, a2);
    
    kprintf("PIC remapped successfully\n");
}

// Send End-Of-Interrupt signal
static void pic_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);  // Send EOI to slave
    }
    outb(0x20, 0x20);      // Send EOI to master
}

// IRQ handler
void irq_handler(registers_t *regs) {
    // Call custom handler if registered
    if (irq_handlers[regs->int_no - 32] != 0) {
        irq_handlers[regs->int_no - 32](regs);
    }
    
    // Send EOI
    pic_eoi(regs->int_no - 32);
}

// Register IRQ handler
void irq_install_handler(int irq, irq_handler_t handler) {
    irq_handlers[irq] = handler;
}

// Unregister IRQ handler
void irq_uninstall_handler(int irq) {
    irq_handlers[irq] = 0;
}

// Install IRQs
void irq_install(void) {
    // Remap PIC
    pic_remap();
    
    // Set IDT gates for IRQs (INT32-47)
    extern void irq0(void);
    extern void irq1(void);
    extern void irq2(void);
    extern void irq3(void);
    extern void irq4(void);
    extern void irq5(void);
    extern void irq6(void);
    extern void irq7(void);
    extern void irq8(void);
    extern void irq9(void);
    extern void irq10(void);
    extern void irq11(void);
    extern void irq12(void);
    extern void irq13(void);
    extern void irq14(void);
    extern void irq15(void);
    
    // Set IDT gates (0x08 is kernel code segment selector, 0x8E is interrupt gate)
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
    
    kprintf("IRQs installed successfully\n");
}
