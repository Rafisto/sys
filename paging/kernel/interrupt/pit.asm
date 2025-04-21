global pit_handler_asm
global page_fault_handler_asm

extern pit_handler
extern page_fault_handler

pit_handler_asm:
    cli                     ; Disable interrupts temporarily for safe handling
    pusha                   ; Save registers
    pushf                   ; Save flags (important for restoring interrupt state)
    call pit_handler        ; Call the handler
    sti                     ; Ensure interrupts are enabled before returning
    popf                    ; Restore flags (interrupt flag should be re-enabled here)
    popa                    ; Restore registers
    iret                    ; Return from interrupt


page_fault_handler_asm:
    cli                     ; Disable interrupts temporarily for safe handling
    pusha                   ; Save registers
    pushf                   ; Save flags (important for restoring interrupt state)
    call page_fault_handler  ; Call the handler
    sti                     ; Ensure interrupts are enabled before returning
    popf                    ; Restore flags (interrupt flag should be re-enabled here)
    popa                    ; Restore registers
    iret                    ; Return from interrupt