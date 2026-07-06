#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT entry structure (packed to match x86 format)
// 64 bits total: offset (32 bits), selector (16 bits), zero (8 bits), type/attr (8 bits)
struct idt_entry_struct {
    uint16_t offset_low;      // Lower 16 bits of handler address
    uint16_t selector;       // Code segment selector
    uint8_t  zero;           // Reserved (must be 0)
    uint8_t  type_attr;      // Type and attributes
    uint16_t offset_high;    // Upper 16 bits of handler address
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// IDT pointer structure for LIDT instruction
struct idt_ptr_struct {
    uint16_t limit;          // Size of IDT - 1
    uint32_t base;           // Base address of IDT
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

// Function to set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Install IDT
void idt_install(void);

#endif // IDT_H
