#include "kprintf.h"
#include "../drivers/vga.h"

// Helper function to convert integer to string (decimal)
static void itoa(int value, char* str, int base) {
    static char digits[] = "0123456789abcdef";
    char* p = str;
    int is_negative = 0;
    
    // Handle negative numbers for decimal
    if (base == 10 && value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    // Handle 0 explicitly
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }
    
    // Convert to string (reversed)
    int temp = value;
    while (temp > 0) {
        *p++ = digits[temp % base];
        temp /= base;
    }
    *p = '\0';
    
    // Reverse the string
    char* start = str;
    char* end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
    
    // Add negative sign if needed
    if (is_negative) {
        // Shift everything right by one
        p = str;
        while (*p != '\0') {
            p++;
        }
        while (p >= str) {
            *(p + 1) = *p;
            p--;
        }
        *str = '-';
    }
}

// Helper function to convert unsigned integer to string
static void utoa(unsigned int value, char* str, int base) {
    static char digits[] = "0123456789abcdef";
    char* p = str;
    
    // Handle 0 explicitly
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }
    
    // Convert to string (reversed)
    unsigned int temp = value;
    while (temp > 0) {
        *p++ = digits[temp % base];
        temp /= base;
    }
    *p = '\0';
    
    // Reverse the string
    char* start = str;
    char* end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
}

// Kernel printf implementation
void kprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[32]; // Buffer for number conversion
    
    while (*format != '\0') {
        if (*format == '%') {
            format++; // Skip '%'
            
            switch (*format) {
                case 's': {
                    // String
                    const char* str = va_arg(args, const char*);
                    while (*str != '\0') {
                        vga_putchar(*str++);
                    }
                    break;
                }
                case 'd': {
                    // Signed integer (decimal)
                    int value = va_arg(args, int);
                    itoa(value, buffer, 10);
                    char* p = buffer;
                    while (*p != '\0') {
                        vga_putchar(*p++);
                    }
                    break;
                }
                case 'u': {
                    // Unsigned integer (decimal)
                    unsigned int value = va_arg(args, unsigned int);
                    utoa(value, buffer, 10);
                    char* p = buffer;
                    while (*p != '\0') {
                        vga_putchar(*p++);
                    }
                    break;
                }
                case 'x': {
                    // Hexadecimal (lowercase)
                    unsigned int value = va_arg(args, unsigned int);
                    utoa(value, buffer, 16);
                    char* p = buffer;
                    while (*p != '\0') {
                        vga_putchar(*p++);
                    }
                    break;
                }
                case 'c': {
                    // Character
                    char c = (char)va_arg(args, int);
                    vga_putchar(c);
                    break;
                }
                case '%': {
                    // Literal percent
                    vga_putchar('%');
                    break;
                }
                default: {
                    // Unknown format specifier, just print it
                    vga_putchar('%');
                    vga_putchar(*format);
                    break;
                }
            }
        } else {
            // Regular character
            vga_putchar(*format);
        }
        format++;
    }
    
    va_end(args);
}
