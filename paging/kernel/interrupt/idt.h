#ifndef KERNEL_INTERRUPT_IDT_H
#define KERNEL_INTERRUPT_IDT_H

#include "../x86.h"
#include "../types.h"
#include "../memory/memory.h"

#include "../../serial/uart.h"
#include "../proc/tasks.h"

#define GDT_KERNEL_CODE 0x08

#define IDT_SIZE 256
#define IDT_ENTRIES 256

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

typedef struct
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_pointer_t;

typedef struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; 
	uint32_t interrupt, error;
	uint32_t eip, cs, eflags, usermode_esp, usermode_ss;
} TrapFrame;

extern void* isr_redirect_table[];
extern void isr128();
extern void keyboard_handler();

void set_idt_entry(uint8_t vector, void* isr, uint8_t attributes);
void idt_init();
void pit_init(uint32_t frequency);

void handle_interrupt(TrapFrame regs);

#define halt() asm volatile("hlt")
#define enable_interrupts() asm volatile("sti")
#define disable_interrupts() asm volatile("cli")

#endif // KERNEL_INTERRUPT_IDT_H
