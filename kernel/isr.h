#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Registers structure pushed by ISR stub
typedef struct {
    uint32_t gs, fs, es, ds;      // Segment registers
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;    // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by CPU automatically
} registers_t;

// Exception handler function
void isr_handler(registers_t *regs);

// Install ISRs
void isr_install(void);

#endif // ISR_H
