// Minimal kernel entry point
// This runs in a freestanding environment with no standard library
// Serial port for headless testing (COM1 port 0x3F8)

// Serial port I/O ports
#define SERIAL_COM1_PORT 0x3F8
#define SERIAL_DATA_REG   (SERIAL_COM1_PORT + 0)  // Data register
#define SERIAL_CMD_REG     (SERIAL_COM1_PORT + 1)  // Command register
#define SERIAL_FIFO_REG   (SERIAL_COM1_PORT + 2)  // FIFO control register
#define SERIAL_LINE_REG   (SERIAL_COM1_PORT + 3)  // Line control register
#define SERIAL_MODEM_REG  (SERIAL_COM1_PORT + 4)  // Modem control register
#define SERIAL_LINE_STATUS_REG (SERIAL_COM1_PORT + 5)  // Line status register

// Inline assembly to read from I/O port
static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

// Inline assembly to write to I/O port
static inline void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

// Function to check if serial port is ready to transmit
static int serial_is_transmit_ready() {
    return inb(SERIAL_LINE_STATUS_REG) & 0x20;
}

// Initialize serial port
void serial_init() {
    outb(SERIAL_LINE_REG, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_DATA_REG, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_CMD_REG, 0x00);     //                  (hi byte)
    outb(SERIAL_LINE_REG, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_FIFO_REG, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(SERIAL_MODEM_REG, 0x0B);   // IRQs enabled, RTS/DSR set
}

// Write character to serial port
void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(SERIAL_DATA_REG, c);
}

// Write string to serial port
void serial_write_string(const char* str) {
    while (*str != '\0') {
        serial_write_char(*str);
        str++;
    }
}

// Kernel main entry point - called from boot.asm
void kernel_main() {
    // Initialize serial port
    serial_init();
    
    // Write "Hello from Nexus OS" to serial port
    serial_write_string("Hello from Nexus OS\r\n");
    
    // Kernel should never return in a real OS
    // If it does, boot.asm will halt the CPU
    while (1) {
        // Infinite loop
    }
}
