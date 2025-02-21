; Multiboot 2 - Compliant Header
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html (Section 3.1.1)
section .multiboot_header
header_start:
    MULTIBOOT_PAGE_ALIGN   equ 1<<0
    MULTIBOOT_HEADER_ARCH  equ 0
    MULTIBOOT_HEADER_MAGIC equ 0xe85250d6
    MULTIBOOT_CHECKSUM     equ 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    MULTIBOOT_FRAMEBUFFER_REQUEST equ 5
    MULTIBOOT_FRAMEBUFFER_TYPE    equ 0
    MULTIBOOT_FRAMEBUFFER_SIZE    equ 20
    MULTIBOOT_FRAMEBUFFER_WIDTH   equ 1024
    MULTIBOOT_FRAMEBUFFER_HEIGHT  equ 768
    MULTIBOOT_FRAMEBUFFER_BPP     equ 32
    
    MULTIBOOT_TYPE  equ 0
    MULTIBOOT_FLAGS equ 0
    MULTIBOOT_SIZE  equ 8

    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_ARCH
    dd header_end - header_start
    dd MULTIBOOT_CHECKSUM

    ; Framebuffer request
    dw MULTIBOOT_FRAMEBUFFER_REQUEST
    dw MULTIBOOT_FRAMEBUFFER_TYPE
    dd MULTIBOOT_FRAMEBUFFER_SIZE
    dd MULTIBOOT_FRAMEBUFFER_WIDTH
    dd MULTIBOOT_FRAMEBUFFER_HEIGHT
    dd MULTIBOOT_FRAMEBUFFER_BPP

    align 8 ; must be 8-byte aligned or GRUB will not recognize it
    dw MULTIBOOT_TYPE
    dw MULTIBOOT_FLAGS
    dd MULTIBOOT_SIZE
header_end:
