#include "idt.h"

idt_entry_t idt_table[IDT_SIZE] __attribute__((aligned(16)));
idt_pointer_t idt_pointer;

static void initialize_pic()
{
    outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

void set_idt_entry(uint8_t vector, void* isr, uint8_t attributes) {
	idt_table[vector].offset_lowerbits    = (uint32_t) isr & 0xFFFF;
	idt_table[vector].selector = GDT_KERNEL_CODE;
	idt_table[vector].zero   = 0;
	idt_table[vector].flags = attributes;
	idt_table[vector].offset_higherbits   = (uint32_t) isr >> 16;
}

void pit_init(uint32_t frequency) {
	uint32_t divisor = 1193180 / frequency;

	outb(0x43, 0x36);

	uint8_t l = (uint8_t) (divisor & 0xFF);
	uint8_t h = (uint8_t) (divisor >> 8 & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}

void idt_init()
{
	initialize_pic();

	// clear IDT
    memset((uint8_t*) &idt_table, 0, sizeof(idt_entry_t) * 256);

	// all our gate types are "32-bit Interrupt Gates"
	// meaning interrupts are disabled when we enter the handler

	for (int i = 0; i < 48; i++) {
		set_idt_entry(i, isr_redirect_table[i], 0x8E);
	}
	set_idt_entry(0x80, isr128, 0xEE); // DPL 3

	idt_pointer.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_pointer.base  = (uint32_t) &idt_table;
	asm("lidt %0" :: "m"(idt_pointer));
}

void handle_interrupt(TrapFrame regs) {
	if (regs.interrupt < 32) {
		// handle CPU exceptions
		slog("CPU Exception %d", regs.interrupt);
		if (regs.interrupt == 13) {
			slog("General Protection Fault");
		} else if (regs.interrupt == 14) {
			slog("Page Fault");
		}
		return;
	}

	if (regs.interrupt == 128) {
		// syscall handler
		slog("Syscall %d", regs.eax);
		return;
	}

	// check if its a IRQ
	if (regs.interrupt >= 32 && regs.interrupt <= 47) {
		// acknowledge IRQ
		if (regs.interrupt >= 40) {
			outb(0xA0, 0x20); // slave PIC
		}
		outb(0x20, 0x20);

		// check if this is a PIT interrupt
		if (regs.interrupt == 32) {
			schedule();
		}
	}
 
	if (regs.interrupt == 0x80) {
		// syscall demonstration
	}
}
