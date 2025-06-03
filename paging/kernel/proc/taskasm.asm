; void switch_context(Task* from, Task* to);
; swaps stack pointer to next task's kernel stack
global switch_context
switch_context:
	mov eax, [esp + 4] ; eax = from
	mov edx, [esp + 8] ; edx = to
	
	; these are the callee-saved registers on x86 according to cdecl
	; they will change once we go off and execute the other task
	; so we need to preserve them for when we get back
	
	; think of it from the perspective of the calling function,
	; it wont notice that we go off and execute some other code
	; but when we return to it, suddenly the registers it
	; expected to remain unchanged has changed
	push ebx
	push esi
	push edi
	push ebp

	; swap kernel stack pointer and store them
	mov [eax + 4], esp ; from->kesp = esp
	mov esp, [edx + 4] ; esp = to->kesp
	
	; NewTaskKernelStack will match the stack from here on out.

	pop ebp
	pop edi
	pop esi
	pop ebx

	ret ; new tasks hijack the return address to new_task_setup

global new_task_setup
new_task_setup:
	; update the segment registers
	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	
	; zero out registers so they dont leak to userspace
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp

	; exit the interrupt, placing us in the real task entry function
	iret
