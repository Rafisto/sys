extern keyboard_handler

global asm_load_idt

global asm_keyboard_handler_interrupt
global asm_read_port
global asm_write_port

asm_load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

asm_keyboard_handler_interrupt:
    pushad
    cld
    call keyboard_handler
    popad
    iretd

asm_read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

asm_write_port:
    mov   edx, [esp + 4]
    mov   al, [esp + 4 + 4]
    out   dx, al
    ret