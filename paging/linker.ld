ENTRY(_start)

SECTIONS {
    . = 0x00100000; /* Load kernel at 1MiB */

    .boot :
    {
        *(.multiboot_header)
    }

    .text : {
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }

    section_end = .;
}
