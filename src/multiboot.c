// src/multiboot.c
#include "multiboot.h"

// This global pointer will be set by boot.asm.
multiboot_info_t* multiboot_info_ptr = 0;
