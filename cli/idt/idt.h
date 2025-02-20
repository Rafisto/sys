#ifndef BASE_IDT_H
#define BASE_IDT_H

#include "../asm/asm.h"

#define IDT_SIZE 256
#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

extern void asm_keyboard_handler_interrupt();
extern void load_idt(void*);

struct idt_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern struct idt_entry idt_table[IDT_SIZE];
extern struct idt_pointer idt_ptr;

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags);
void interrupt_descriptor_table_init();

#endif // BASE_IDT_H
