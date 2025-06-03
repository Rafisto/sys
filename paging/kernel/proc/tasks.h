#ifndef KERNEL_PROC_TASKS_H
#define KERNEL_PROC_TASKS_H

#include "../types.h"
#include "../gdt/gdt.h"
#include "../../serial/uart.h"

#define MAX_TASKS 16

typedef struct {
	uint32_t ebp, edi, esi, ebx;
	uint32_t switch_context_return_addr;
	uint32_t data_selector;
	uint32_t eip, cs, eflags, usermode_esp, usermode_ss;
} NewTaskKernelStack;

typedef struct {
	uint32_t id;
	uint32_t kesp;
	uint32_t kesp_bottom;
} Task;

extern tss_t tss;
void schedule();
void setup_tasks();
void create_task(uint32_t id, uint32_t eip, uint32_t user_stack, uint32_t kernel_stack, int is_kernel_task);
extern void switch_context(Task* from, Task* to);
extern void new_task_setup();

#endif