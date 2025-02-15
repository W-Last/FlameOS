#include "multiboot.h"
#include "print.h"
#include "fs.h"
#include <stdint.h>

void kernel_main(void) {
    if (multiboot_info_ptr) {
        printf("mods_count: %d\n", multiboot_info_ptr->mods_count);
        if (multiboot_info_ptr->mods_count > 0) {
            multiboot_module_t* modules = (multiboot_module_t*)(multiboot_info_ptr->mods_addr);
            uint32_t archive_start = modules[0].mod_start;
            uint32_t archive_end   = modules[0].mod_end;
            parse_cpio_archive((void*)archive_start, archive_end - archive_start);
        } else {
            printf("No modules found!\n");
        }
    } else {
        printf("multiboot_info_ptr is NULL!\n");
    }
    while (1) { }
}

