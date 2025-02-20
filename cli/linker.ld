ENTRY(main)

SECTIONS {
    . = 0x100000; /* Tells GRUB to load the kernel starting at the 1MiB mark */

    .boot :
    {
        /* Ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }

    .text : {
	    start_text = .;
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
        end_data = .;
    }

    .bss : {
        start_bss = .;
        *(COMMON)
        *(.bss)
        end_bss = .;  /* End of .bss section */
    }

    end = .;
}
