# Makefile for Nexus OS kernel
# This builds the kernel and provides a target to boot it in QEMU

# Toolchain configuration
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-gcc
QEMU = qemu-system-i386

# Compiler flags for freestanding environment
CFLAGS = -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-builtin -fno-exceptions -fno-stack-protector -m32
LDFLAGS = -ffreestanding -O2 -nostdlib -m32

# Directories
BUILD_DIR = build
BOOT_DIR = boot
KERNEL_DIR = kernel
DRIVERS_DIR = drivers

# Files
BOOT_ASM = $(BOOT_DIR)/boot.asm
KERNEL_C = $(KERNEL_DIR)/kernel.c
KPRINTF_C = $(KERNEL_DIR)/kprintf.c
STRING_C = $(KERNEL_DIR)/string.c
SHELL_C = $(KERNEL_DIR)/shell.c
GDT_C = $(KERNEL_DIR)/gdt.c
IDT_C = $(KERNEL_DIR)/idt.c
ISR_C = $(KERNEL_DIR)/isr.c
IRQ_C = $(KERNEL_DIR)/irq.c
VGA_C = $(DRIVERS_DIR)/vga.c
TIMER_C = $(DRIVERS_DIR)/timer.c
KEYBOARD_C = $(DRIVERS_DIR)/keyboard.c
GDT_FLUSH_ASM = $(KERNEL_DIR)/gdt_flush.asm
IDT_FLUSH_ASM = $(KERNEL_DIR)/idt_flush.asm
ISR_STUBS_ASM = $(KERNEL_DIR)/isr_stubs.asm
IRQ_STUBS_ASM = $(KERNEL_DIR)/irq_stubs.asm
LINKER_SCRIPT = $(BOOT_DIR)/linker.ld

# Output files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KPRINTF_OBJ = $(BUILD_DIR)/kprintf.o
STRING_OBJ = $(BUILD_DIR)/string.o
SHELL_OBJ = $(BUILD_DIR)/shell.o
GDT_OBJ = $(BUILD_DIR)/gdt.o
IDT_OBJ = $(BUILD_DIR)/idt.o
ISR_OBJ = $(BUILD_DIR)/isr.o
IRQ_OBJ = $(BUILD_DIR)/irq.o
VGA_OBJ = $(BUILD_DIR)/vga.o
TIMER_OBJ = $(BUILD_DIR)/timer.o
KEYBOARD_OBJ = $(BUILD_DIR)/keyboard.o
GDT_FLUSH_OBJ = $(BUILD_DIR)/gdt_flush.o
IDT_FLUSH_OBJ = $(BUILD_DIR)/idt_flush.o
ISR_STUBS_OBJ = $(BUILD_DIR)/isr_stubs.o
IRQ_STUBS_OBJ = $(BUILD_DIR)/irq_stubs.o
KERNEL_BIN = $(BUILD_DIR)/nexus.bin

# Default target
all: $(KERNEL_BIN)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble boot.asm
$(BOOT_OBJ): $(BOOT_ASM) $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Compile kernel.c
$(KERNEL_OBJ): $(KERNEL_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile kprintf.c
$(KPRINTF_OBJ): $(KPRINTF_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile vga.c
$(VGA_OBJ): $(VGA_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile gdt.c
$(GDT_OBJ): $(GDT_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile idt.c
$(IDT_OBJ): $(IDT_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile isr.c
$(ISR_OBJ): $(ISR_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile irq.c
$(IRQ_OBJ): $(IRQ_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile timer.c
$(TIMER_OBJ): $(TIMER_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile keyboard.c
$(KEYBOARD_OBJ): $(KEYBOARD_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile string.c
$(STRING_OBJ): $(STRING_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile shell.c
$(SHELL_OBJ): $(SHELL_C) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble gdt_flush.asm
$(GDT_FLUSH_OBJ): $(GDT_FLUSH_ASM) $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Assemble idt_flush.asm
$(IDT_FLUSH_OBJ): $(IDT_FLUSH_ASM) $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Assemble isr_stubs.asm
$(ISR_STUBS_OBJ): $(ISR_STUBS_ASM) $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Assemble irq_stubs.asm
$(IRQ_STUBS_OBJ): $(IRQ_STUBS_ASM) $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Link kernel
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(KPRINTF_OBJ) $(STRING_OBJ) $(SHELL_OBJ) $(GDT_OBJ) $(IDT_OBJ) $(ISR_OBJ) $(IRQ_OBJ) $(VGA_OBJ) $(TIMER_OBJ) $(KEYBOARD_OBJ) $(GDT_FLUSH_OBJ) $(IDT_FLUSH_OBJ) $(ISR_STUBS_OBJ) $(IRQ_STUBS_OBJ) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(BOOT_OBJ) $(KERNEL_OBJ) $(KPRINTF_OBJ) $(STRING_OBJ) $(SHELL_OBJ) $(GDT_OBJ) $(IDT_OBJ) $(ISR_OBJ) $(IRQ_OBJ) $(VGA_OBJ) $(TIMER_OBJ) $(KEYBOARD_OBJ) $(GDT_FLUSH_OBJ) $(IDT_FLUSH_OBJ) $(ISR_STUBS_OBJ) $(IRQ_STUBS_OBJ) -o $@

# Run kernel in QEMU
run: $(KERNEL_BIN)
	$(QEMU) -kernel $(KERNEL_BIN) -curses

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all run clean
