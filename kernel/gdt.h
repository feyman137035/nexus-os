#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT entry structure (packed to match x86 format)
// 64 bits total: base (32 bits), limit (20 bits), access (8 bits), granularity (4 bits)
struct gdt_entry_struct {
    uint16_t limit_low;     // Lower 16 bits of limit
    uint16_t base_low;      // Lower 16 bits of base
    uint8_t  base_middle;   // Middle 8 bits of base
    uint8_t  access;        // Access byte (present, ring level, type)
    uint8_t  granularity;   // Granularity and upper 4 bits of limit
    uint8_t  base_high;     // Upper 8 bits of base
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

// GDT pointer structure for LGDT instruction
struct gdt_ptr_struct {
    uint16_t limit;         // Size of GDT - 1
    uint32_t base;          // Base address of GDT
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

// Function to set a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// Install GDT
void gdt_install(void);

#endif // GDT_H
