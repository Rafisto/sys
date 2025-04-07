#ifndef BASE_ASM_H
#define BASE_ASM_H

// idt.asm
extern void asm_keyboard_handler_interrupt(void);
extern unsigned char asm_read_port(int port);
extern void asm_write_port(int port, unsigned char val);
extern void asm_load_idt(void* ptr);

// power.asm
extern void asm_shutdown(void);
extern void asm_reboot(void);

// paging.asm
extern void asm_load_page_directory(unsigned int* page_directory);
extern void asm_enable_paging(void);

// outb (inb) - write (read) byte to (from) port
static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // BASE_ASM_H