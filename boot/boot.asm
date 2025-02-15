; boot/boot.asm
[bits 32]
[global _start]
[extern kernel_main]
[extern multiboot_info_ptr]

section .multiboot
    ; Multiboot v1 header (must be in the first 8 KB of the file)
    dd 0x1BADB002            ; MULTIBOOT_HEADER_MAGIC
    dd 0x00000000            ; flags (0 for now)
    dd -(0x1BADB002 + 0x00000000)  ; checksum

section .text
_start:
    mov dword [multiboot_info_ptr], eax
    call kernel_main

hang:
    cli
    hlt
    jmp hang
