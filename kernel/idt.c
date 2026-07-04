#include "idt.h"
#include "kprintf.h"

// IDT entries (256 entries for all possible interrupts)
idt_entry_t idt_entries[256];
// IDT pointer
idt_ptr_t idt_ptr;

// External assembly function to load IDT
extern void idt_flush(uint32_t idt_ptr_address);

// Set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    // Set offset (handler address)
    idt_entries[num].offset_low = base & 0xFFFF;
    idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
    
    // Set selector (code segment)
    idt_entries[num].selector = sel;
    
    // Set zero
    idt_entries[num].zero = 0;
    
    // Set type and attributes
    idt_entries[num].type_attr = flags;
}

// Install IDT
void idt_install(void) {
    // Setup IDT pointer
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    
    // Clear IDT (set all entries to 0)
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }
    
    // Load IDT
    idt_flush((uint32_t)&idt_ptr);
    
    kprintf("IDT installed successfully\n");
}
