global asm_shutdown
global asm_reboot

asm_shutdown:
    ; ACPI shutdown code
    mov eax, 0xF4
    out dx, eax

    ; Qemu shutdown code (older versions)
    mov dx, 0xB004
    mov ax, 0x2000
    out dx, ax

    ; Qemu shutdown code (newer versions)
    mov dx, 0x604
    mov ax, 0x2000
    out dx, ax

asm_reboot:
    ; ACPI reboot code
    jmp 0xFFFF:0