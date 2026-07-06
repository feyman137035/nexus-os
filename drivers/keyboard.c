#include "keyboard.h"
#include "../kernel/irq.h"
#include "../kernel/kprintf.h"

#define KEYBOARD_BUFFER_SIZE 256

// Circular buffer
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile int buffer_head = 0;
static volatile int buffer_tail = 0;

// Shift state
static int shift_pressed = 0;

// Port I/O helpers
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

// US QWERTY Keyboard Map (Set 1 Make Codes)
static const char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 0-9 */
  '9', '0', '-', '=', '\b',                            /* 10-14: Backspace */
  '\t',                                                /* 15: Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',    /* 16-25 */
  '[', ']', '\n',                                      /* 26-28: Enter */
    0,                                                 /* 29: Ctrl */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    /* 30-39 */
 '\'', '`',   0,                                       /* 40-42: Left Shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',    /* 43-52 */
  '/',   0,                                            /* 53-54: Right Shift */
  '*',
    0,                                                 /* 56: Alt */
  ' ',                                                 /* 57: Space */
    0,                                                 /* 58: Caps Lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* F1-F10 */
    0,                                                 /* 69: Num Lock */
    0,                                                 /* 70: Scroll Lock */
    0,                                                 /* 71: Home */
    0,                                                 /* 72: Up Arrow */
    0,                                                 /* 73: Page Up */
  '-',
    0,                                                 /* 75: Left Arrow */
    0,
    0,                                                 /* 77: Right Arrow */
  '+',
    0,                                                 /* 79: End */
    0,                                                 /* 80: Down Arrow */
    0,                                                 /* 81: Page Down */
    0,                                                 /* 82: Insert */
    0,                                                 /* 83: Delete */
    0, 0, 0,
    0,                                                 /* 87: F11 */
    0,                                                 /* 88: F12 */
    0                                                  /* Rest undefined */
};

// US QWERTY Keyboard Map with Shift
static const char keyboard_shift_map[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 0-9 */
  '(', ')', '_', '+', '\b',                            /* 10-14: Backspace */
  '\t',                                                /* 15: Tab */
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',    /* 16-25 */
  '{', '}', '\n',                                      /* 26-28: Enter */
    0,                                                 /* 29: Ctrl */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',    /* 30-39 */
  '\"', '~',   0,                                      /* 40-42: Left Shift */
  '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>',    /* 43-52 */
  '?',   0,                                            /* 53-54: Right Shift */
  '*',
    0,                                                 /* 56: Alt */
  ' ',                                                 /* 57: Space */
    0,                                                 /* 58: Caps Lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* F1-F10 */
    0,                                                 /* 69: Num Lock */
    0,                                                 /* 70: Scroll Lock */
    0,                                                 /* 71: Home */
    0,                                                 /* 72: Up Arrow */
    0,                                                 /* 73: Page Up */
  '-',
    0,                                                 /* 75: Left Arrow */
    0,
    0,                                                 /* 77: Right Arrow */
  '+',
    0,                                                 /* 79: End */
    0,                                                 /* 80: Down Arrow */
    0,                                                 /* 81: Page Down */
    0,                                                 /* 82: Insert */
    0,                                                 /* 83: Delete */
    0, 0, 0,
    0,                                                 /* 87: F11 */
    0,                                                 /* 88: F12 */
    0                                                  /* Rest undefined */
};

// Keyboard Interrupt Handler
static void keyboard_handler(registers_t *regs) {
    (void)regs; // Unused parameter

    // Read the scancode from the PS/2 controller data port
    uint8_t scancode = inb(0x60);

    // Track shift state (press: left=0x2A, right=0x36; release: left=0xAA, right=0xB6)
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    } else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    // Ignore key release events (break codes, where high bit is set)
    if (scancode & 0x80) {
        return;
    }

    // Convert scancode to ASCII
    char ascii = 0;
    if (scancode < 128) {
        ascii = shift_pressed ? keyboard_shift_map[scancode] : keyboard_map[scancode];
    }

    // Only put valid ASCII characters into buffer
    if (ascii != 0) {
        int next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        if (next != buffer_tail) {
            keyboard_buffer[buffer_head] = ascii;
            buffer_head = next;
        }
    }
}

// Initialize keyboard
void keyboard_init(void) {
    // Register our handler on IRQ1 (IRQ number 1)
    irq_install_handler(1, keyboard_handler);
    kprintf("Keyboard driver initialized\n");
}

// Non-blocking read
int32_t keyboard_read_char(void) {
    if (buffer_head == buffer_tail) {
        return -1; // Empty buffer
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

// Blocking read
char keyboard_getchar(void) {
    int32_t c;
    while ((c = keyboard_read_char()) == -1) {
        __asm__ volatile("hlt"); // Rest the CPU until next interrupt
    }
    return (char)c;
}
