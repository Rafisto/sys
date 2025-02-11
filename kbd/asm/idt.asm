extern keyboard_handler

global load_idt

global keyboard_handler_interrupt
global read_port
global write_port

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

keyboard_handler_interrupt:
    pushad
    cld
    call keyboard_handler
    popad
    iretd

read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

write_port:
    mov   edx, [esp + 4]
    mov   al, [esp + 4 + 4]
    out   dx, al
    ret