bits   32
global start
extern kmain

global stack_begin
global stack_end

start:    
    lgdt [gdtr]

    jmp CODE32_SEL:.setcs
.setcs:
    mov ax,  DATA32_SEL
    mov ds,  ax
    mov es,  ax
    mov fs,  ax
    mov gs,  ax
    mov ss,  ax
    mov esp, stack_end

    ; Enable SSE and FPU
    ; Necessary as otherwise executing FPA 
    ; instructions will cause a general protection fault
    mov eax, cr0
    and eax, 0xFFFFFFFB  ; Clear bit 2 to enable FPU
    or eax, 0x2          ; Set bit 1 to enable SSE
    mov cr0, eax
    mov eax, cr4
    or eax, 3 << 9       ; Set OSFXSR and OSXMMEXCPT bits
    mov cr4, eax

    push ebx
    call kmain

endloop:
    hlt                         
    jmp endloop

%define MAKE_GDT_DESC(base, limit, access, flags) \
    (((base  & 0x00FFFFFF) << 16) | \
    ((base   & 0xFF000000) << 32) | \
    (limit   & 0x0000FFFF) | \
    ((limit  & 0x000F0000) << 32) | \
    ((access & 0xFF) << 40) | \
    ((flags  & 0x0F) << 52))

section .data
align   4
gdt_start:
    dq MAKE_GDT_DESC(0, 0, 0, 0)
gdt32_code:
    dq MAKE_GDT_DESC(0, 0x00ffffff, 10011010b, 1100b)                           
gdt32_data:
    dq MAKE_GDT_DESC(0, 0x00ffffff, 10010010b, 1100b)
                               
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1
                                
    dd gdt_start

CODE32_SEL equ gdt32_code - gdt_start
DATA32_SEL equ gdt32_data - gdt_start

section .bss

stack_begin:
    resb 4 * 1024 * 1024; 1MiB stack
stack_end: