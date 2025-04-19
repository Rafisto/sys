bits   32
global _start

extern multiboot_info ; Multiboot info structure (boot/multiboot.h)
extern kmain          ; C entrypoint function (kernel/kmain.c)
extern entrypgdir     ; Page directory address from C (kernel/memory/paging.h)

%define CR4_PSE    0x00000010 ; Page Size Extension
%define CR0_PAGING 0x80000000 ; Paging Enable

section .text
_start:
    ; Read the multiboot info structure
    push ebx
    call multiboot_info

    ; Enable Page Size Extension (for 4 MiB pages if needed)
    mov eax, cr4
    or  eax, CR4_PSE
    mov cr4, eax

    ; Load the address of the page directory
    mov eax, entrypgdir
    mov cr3, eax

    ; Enable paging
    mov eax, cr0
    or  eax, CR0_PAGING
    mov cr0, eax

    ; Jump to kernel entry
    call kmain
