#include "tasks.h"

Task tasks[MAX_TASKS];
int num_tasks;
Task* current_task;

void setup_tasks() {
	memset((uint8_t*) tasks, 0, sizeof(Task) * MAX_TASKS);

	// task 0 represents the execution we're in right now
	// (the kernel/idle thread)
	num_tasks = 1;
	current_task = &tasks[0];
	current_task->id = 0;
}

// you can also create "kernel tasks" running in kernel space
// they dont need a user stack, instead using its kernel stack for both program code and interrupts
void create_task(uint32_t id, uint32_t eip, uint32_t user_stack, uint32_t kernel_stack, int is_kernel_task) {
	num_tasks++;

	if (!is_kernel_task) {
		// we can pass things to the task by pushing to its user stack
		// with cdecl, this will pass it as arguments
		user_stack -= 4;
		*(uint32_t*) user_stack = id; // first arg to task func
		user_stack -= 4;
		*(uint32_t*) user_stack = 0; // task func return address, shouldnt be used
	}

	uint32_t code_selector = is_kernel_task ? GDT_KERNEL_CODE : (GDT_USER_CODE | RPL_USER);
	uint32_t data_selector = is_kernel_task ? GDT_KERNEL_DATA : (GDT_USER_DATA | RPL_USER);

	uint8_t* kesp = (uint8_t*) kernel_stack;

	// we need to set up the initial kernel stack for this task
	// this stack will be loaded next time switch_context gets
	// called for this task
	// once switch_context switches esp to this stack, the ret
	// instruction will pop off a return value, so we redirect it
	// to new_task_setup to init registers and exit the interrupt
	kesp -= sizeof(NewTaskKernelStack);
	NewTaskKernelStack* stack = (NewTaskKernelStack*) kesp;
	stack->ebp = stack->edi = stack->esi = stack->ebx = 0;
	stack->switch_context_return_addr = (uint32_t) new_task_setup;
	stack->data_selector = data_selector;
	stack->eip = eip;
	stack->cs = code_selector;
	stack->eflags = 0x200;

	stack->usermode_esp = user_stack; 
	stack->usermode_ss = data_selector;

	tasks[id].kesp_bottom = kernel_stack;
	tasks[id].kesp = (uint32_t) kesp;
	tasks[id].id = id;
}

void schedule() {
	int next_id = (current_task->id + 1) % num_tasks;

	Task* next = &tasks[next_id];
	Task* old = current_task;
	current_task = next;

	tss.esp0 = next->kesp_bottom;

	// switch context, doesn't return here for newly created tasks
	switch_context(old, next);
}

// FIXME: cant pass args to kernel tasks
static void task_entry(uint32_t id) {
	asm("int $0x80" :: "a"(id));

	uint8_t a = 0;
	while (1) {
		sprintf("Task %d running, a=%d\n", id, a++);
	}

	// IMPORTANT: all tasks need to end in an infinite loop, otherwise
	// the cpu will just continue executing garbage code from here
}