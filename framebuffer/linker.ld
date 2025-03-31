ENTRY(main)

SECTIONS {
    . = 0x100000;

    .boot :
    {
        *(.multiboot_header)
    }

    .text : {
        *(.text)
    }
}
