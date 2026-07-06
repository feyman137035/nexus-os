#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"

// IRQ handler function pointer type
typedef void (*irq_handler_t)(registers_t *regs);

// Install IRQ handlers
void irq_install(void);

// Remap PIC
void pic_remap(void);

// Register a handler for a specific IRQ
void irq_install_handler(int irq, irq_handler_t handler);

// Unregister a handler for a specific IRQ
void irq_uninstall_handler(int irq);

#endif // IRQ_H
