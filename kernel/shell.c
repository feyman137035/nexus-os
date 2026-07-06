#include "shell.h"
#include "string.h"
#include "kprintf.h"
#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"

// Port I/O helper for rebooting
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

void shell_run(void) {
    char cmd_buf[256];
    int cmd_len = 0;

    kprintf("Welcome to Nexus OS Shell!\n");
    kprintf("Type 'help' for a list of available commands.\n\n");

    while (1) {
        kprintf("nexus> ");
        cmd_len = 0;
        
        while (1) {
            char c = keyboard_getchar();
            
            // Handle Enter key
            if (c == '\n') {
                kprintf("\n");
                cmd_buf[cmd_len] = '\0';
                break;
            }
            
            // Handle Backspace
            if (c == '\b') {
                if (cmd_len > 0) {
                    cmd_len--;
                    kprintf("\b \b");
                }
                continue;
            }
            
            // Handle other printable characters
            if (c >= 32 && c <= 126) {
                if (cmd_len < 254) {
                    cmd_buf[cmd_len++] = c;
                    kprintf("%c", c);
                }
            }
        }
        
        // Command Dispatcher
        if (strcmp(cmd_buf, "help") == 0) {
            kprintf("Available commands:\n");
            kprintf("  help          - Show this help message\n");
            kprintf("  clear         - Clear the screen\n");
            kprintf("  echo <text>   - Print text back to the screen\n");
            kprintf("  ticks         - Print the current timer tick count\n");
            kprintf("  reboot        - Reboot the CPU\n");
        } else if (strcmp(cmd_buf, "clear") == 0) {
            vga_clear();
        } else if (strncmp(cmd_buf, "echo ", 5) == 0) {
            kprintf("%s\n", cmd_buf + 5);
        } else if (strcmp(cmd_buf, "echo") == 0) {
            kprintf("\n");
        } else if (strcmp(cmd_buf, "ticks") == 0) {
            kprintf("Ticks: %u\n", timer_get_ticks());
        } else if (strcmp(cmd_buf, "reboot") == 0) {
            kprintf("Rebooting system...\n");
            // Send Reset CPU command (pulse line) to 8042 controller command port
            outb(0x64, 0xFE);
            // Fallback triple-fault (Invalid opcode triggers Exception 6 -> GPF -> double fault -> triple fault)
            __asm__ volatile("ud2");
        } else if (cmd_len == 0) {
            // Empty line, do nothing
        } else {
            kprintf("Unknown command: %s\n", cmd_buf);
        }
    }
}
