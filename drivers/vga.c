#include "vga.h"

// VGA buffer pointer (volatile to prevent compiler optimization)
static volatile uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;

// Current cursor position (static state inside driver)
static int cursor_row = 0;
static int cursor_col = 0;

// Current color (foreground in lower 4 bits, background in upper 4 bits)
static uint8_t current_color = 0x0F; // White on black by default

// Helper function to create a VGA entry (character + color)
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)(color << 8);
}

// Initialize VGA driver
void vga_initialize(void) {
    cursor_row = 0;
    cursor_col = 0;
    current_color = 0x0F; // White on black
    vga_clear();
}

// Set the current color for subsequent writes
void vga_setcolor(uint8_t fg, uint8_t bg) {
    // Combine foreground (lower 4 bits) and background (upper 4 bits)
    current_color = fg | (bg << 4);
}

// Clear the entire screen
void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', current_color);
    }
    cursor_row = 0;
    cursor_col = 0;
}

// Scroll the screen up by one line
static void vga_scroll(void) {
    // Move each row up by one
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int src_index = row * VGA_WIDTH + col;
            int dst_index = (row - 1) * VGA_WIDTH + col;
            vga_buffer[dst_index] = vga_buffer[src_index];
        }
    }
    
    // Clear the last row
    int last_row_start = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (int col = 0; col < VGA_WIDTH; col++) {
        vga_buffer[last_row_start + col] = vga_entry(' ', current_color);
    }
    
    // Move cursor to the last row
    cursor_row = VGA_HEIGHT - 1;
    cursor_col = 0;
}

// Write a single character to VGA buffer at current cursor position
void vga_putchar(char c) {
    // Handle newline
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        
        // Scroll if we've reached the bottom
        if (cursor_row >= VGA_HEIGHT) {
            vga_scroll();
        }
        return;
    }
    
    // Handle tab (4 spaces)
    if (c == '\t') {
        for (int i = 0; i < 4; i++) {
            vga_putchar(' ');
        }
        return;
    }
    
    // Handle carriage return
    if (c == '\r') {
        cursor_col = 0;
        return;
    }
    
    // Bounds checking - don't write outside buffer
    if (cursor_row >= VGA_HEIGHT || cursor_col >= VGA_WIDTH) {
        return;
    }
    
    // Write character to buffer
    int index = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[index] = vga_entry(c, current_color);
    
    // Advance cursor
    cursor_col++;
    
    // Handle line wrap
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        
        // Scroll if we've reached the bottom
        if (cursor_row >= VGA_HEIGHT) {
            vga_scroll();
        }
    }
}
