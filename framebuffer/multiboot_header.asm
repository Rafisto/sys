; Multiboot 2 - Compliant Header
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html (Section 3.1.1)
section .multiboot_header
header_start:
    ; magic number used for multiboot2
    dd 0xe85250d6
    ; architecture
    dd 0 ; protected mode i386
    ; header length
    dd header_end - header_start
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
    ; ===
    dw 5
    dw 0
    dd 20
    dd 1024
    dd 768
    dd 32

    ; end tag
    ;dw 0
    ;dw 0
    ;dd 8
header_end:

