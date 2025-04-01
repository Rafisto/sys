#ifndef BASE_ASM_H
#define BASE_ASM_H

// idt.asm
extern void asm_keyboard_handler_interrupt(void);
extern void asm_divide_by_zero_handler(void);
extern unsigned char asm_read_port(int port);
extern void asm_write_port(int port, unsigned char val);
extern void asm_load_idt(void* ptr);

// power.asm
extern void asm_shutdown(void);
extern void asm_reboot(void);

// paging.asm
extern void asm_load_page_directory(unsigned int* page_directory);
extern void asm_enable_paging(void);

#endif // BASE_ASM_H