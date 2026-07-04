#include "gdt.h"
#include "kprintf.h"

// GDT entries (5 entries: null, kernel code, kernel data, user code, user data)
gdt_entry_t gdt_entries[5];
// GDT pointer
gdt_ptr_t gdt_ptr;

// External assembly function to load GDT and reload segment registers
extern void gdt_flush(uint32_t gdt_ptr_address);

// Set a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Set base address
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    
    // Set limit
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    
    // Set granularity and flags
    gdt_entries[num].granularity |= gran & 0xF0;
    
    // Set access byte
    gdt_entries[num].access = access;
}

// Install GDT
void gdt_install(void) {
    // Setup GDT pointer
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;
    
    // Null descriptor (required by x86 spec)
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Kernel code segment (base=0, limit=0xFFFFFFFF, present, ring 0, code, executable, readable)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Kernel data segment (base=0, limit=0xFFFFFFFF, present, ring 0, data, writable, grow-up)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    // User code segment (base=0, limit=0xFFFFFFFF, present, ring 3, code, executable, readable)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    
    // User data segment (base=0, limit=0xFFFFFFFF, present, ring 3, data, writable, grow-up)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    
    // Load GDT
    gdt_flush((uint32_t)&gdt_ptr);
    
    kprintf("GDT installed successfully\n");
}
