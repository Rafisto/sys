#ifndef BASE_ASM_H
#define BASE_ASM_H

// idt.asm
extern void keyboard_handler_interrupt(void);
extern unsigned char read_port(int port);
extern void write_port(int port, unsigned char val);

#endif // BASE_ASM_H