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
VGA_C = $(DRIVERS_DIR)/vga.c
LINKER_SCRIPT = $(BOOT_DIR)/linker.ld

# Output files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KPRINTF_OBJ = $(BUILD_DIR)/kprintf.o
VGA_OBJ = $(BUILD_DIR)/vga.o
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

# Link kernel
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(KPRINTF_OBJ) $(VGA_OBJ) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(BOOT_OBJ) $(KERNEL_OBJ) $(KPRINTF_OBJ) $(VGA_OBJ) -o $@

# Run kernel in QEMU
run: $(KERNEL_BIN)
	$(QEMU) -kernel $(KERNEL_BIN) -nographic -serial file:serial.log

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all run clean
