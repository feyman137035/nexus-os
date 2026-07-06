#include "isr.h"
#include "idt.h"
#include "kprintf.h"

// Exception messages
static const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// External assembly ISR handler
extern void isr_common_stub(void);

// Serial write helpers
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

// Exception handler
void isr_handler(registers_t *regs) {
    kprintf("Exception %d: %s\n", regs->int_no, exception_messages[regs->int_no]);
    
    serial_write_string("Exception handler: trapped CPU exception\r\n");
    serial_write_string("Exception name: ");
    serial_write_string(exception_messages[regs->int_no]);
    serial_write_string("\r\n");
    
    // Halt the CPU (no recovery yet)
    while (1) {
        __asm__ volatile("hlt");
    }
}

// Install ISRs
void isr_install(void) {
    // Set IDT gates for all 32 CPU exceptions
    // Each ISR stub will be defined in assembly
    extern void isr0(void);
    extern void isr1(void);
    extern void isr2(void);
    extern void isr3(void);
    extern void isr4(void);
    extern void isr5(void);
    extern void isr6(void);
    extern void isr7(void);
    extern void isr8(void);
    extern void isr9(void);
    extern void isr10(void);
    extern void isr11(void);
    extern void isr12(void);
    extern void isr13(void);
    extern void isr14(void);
    extern void isr15(void);
    extern void isr16(void);
    extern void isr17(void);
    extern void isr18(void);
    extern void isr19(void);
    extern void isr20(void);
    extern void isr21(void);
    extern void isr22(void);
    extern void isr23(void);
    extern void isr24(void);
    extern void isr25(void);
    extern void isr26(void);
    extern void isr27(void);
    extern void isr28(void);
    extern void isr29(void);
    extern void isr30(void);
    extern void isr31(void);
    
    // Set IDT gates (0x08 is kernel code segment selector, 0x8E is interrupt gate)
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    
    kprintf("ISRs installed successfully\n");
}
