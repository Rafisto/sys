global load_gdt
load_gdt:
	mov eax, [esp + 4]
	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; do a long jump to load CS with correct value
	; RW nasm align 8

	jmp 0x08:.load_cs
.load_cs:
	ret