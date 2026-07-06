#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../kernel/isr.h"

// Initialize keyboard driver (registers IRQ1 handler)
void keyboard_init(void);

// Read character from circular buffer (non-blocking, returns -1 if empty)
int32_t keyboard_read_char(void);

// Read character from circular buffer (blocking)
char keyboard_getchar(void);

#endif // KEYBOARD_H
