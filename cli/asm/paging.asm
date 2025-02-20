global asm_load_page_directory
global asm_enable_paging

asm_load_page_directory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

asm_enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret