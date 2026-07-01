#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdarg.h>

// Kernel printf function - writes to VGA
void kprintf(const char* format, ...);

#endif // KPRINTF_H
