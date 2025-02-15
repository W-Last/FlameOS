/* link.ld */
ENTRY(_start)

SECTIONS
{
    . = 0x100000; /* load at 1 MB */

    .text : {
        *(.multiboot)   /* must appear first */
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*) *(COMMON)
    }
}
