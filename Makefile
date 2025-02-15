# Makefile

# Tools
ASM = nasm
CC = gcc
GRUB_MKRESCUE = grub-mkrescue

# Flags (Removed -nostdinc so that stdarg.h can be found)
CFLAGS = -m32 -ffreestanding -O2 -Wall -nostdlib -fno-builtin -static -fno-pie -fno-pic
LDFLAGS = -m32 -T link.ld -nostdlib -static -fno-pie -fno-pic


# Directories
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/iso
BOOT_DIR = boot
SRC_DIR = src
MAIN_ROOT_DIR = main_root

# Targets
all: $(BUILD_DIR)/FlameOS.iso

# Build the main_root.cpio archive into the build folder.
$(BUILD_DIR)/main_root.cpio: $(BUILD_DIR) $(MAIN_ROOT_DIR)/*
	cd $(MAIN_ROOT_DIR) && find . | cpio -o --format=newc > ../$(BUILD_DIR)/main_root.cpio

# Compile multiboot.c
$(SRC_DIR)/multiboot.o: $(SRC_DIR)/multiboot.c $(SRC_DIR)/multiboot.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/multiboot.c -o $(SRC_DIR)/multiboot.o

# Compile fs.c
$(SRC_DIR)/fs.o: $(SRC_DIR)/fs.c $(SRC_DIR)/fs.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/fs.c -o $(SRC_DIR)/fs.o

# Assemble the boot code
$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.asm
	$(ASM) -f elf32 -o $(BOOT_DIR)/boot.o $(BOOT_DIR)/boot.asm

# Compile the kernel C code
$(SRC_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/kernel.c -o $(SRC_DIR)/kernel.o

# Compile print.c
$(SRC_DIR)/print.o: $(SRC_DIR)/print.c $(SRC_DIR)/print.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/print.c -o $(SRC_DIR)/print.o

# Build the kernel ELF binary
$(BUILD_DIR)/FlameOS.elf: $(BOOT_DIR)/boot.o $(SRC_DIR)/kernel.o $(SRC_DIR)/print.o $(SRC_DIR)/fs.o $(SRC_DIR)/multiboot.o link.ld | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $(BUILD_DIR)/FlameOS.elf $(BOOT_DIR)/boot.o $(SRC_DIR)/kernel.o $(SRC_DIR)/print.o $(SRC_DIR)/fs.o $(SRC_DIR)/multiboot.o

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create the ISO directory structure
$(ISO_DIR)/boot/grub:
	mkdir -p $(ISO_DIR)/boot/grub

# Create the bootable ISO image using GRUB.
$(BUILD_DIR)/FlameOS.iso: $(BUILD_DIR)/FlameOS.elf grub.cfg $(BUILD_DIR)/main_root.cpio | $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/FlameOS.elf $(ISO_DIR)/boot/FlameOS.elf
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	cp $(BUILD_DIR)/main_root.cpio $(ISO_DIR)/boot/main_root.cpio
	$(GRUB_MKRESCUE) -o $(BUILD_DIR)/FlameOS.iso $(ISO_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BOOT_DIR)/*.o $(SRC_DIR)/*.o

.PHONY: all clean
