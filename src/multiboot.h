/* multiboot.h */
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

typedef struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;    // Points to the module command-line string
    uint32_t reserved;
} multiboot_module_t;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    // (Other fields can be added as needed)
} multiboot_info_t;

// Declare the global pointer. The boot code should set this pointer.
extern multiboot_info_t* multiboot_info_ptr;

#endif
