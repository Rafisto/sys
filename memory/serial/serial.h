#ifndef SERIAL_SERIAL_H
#define SERIAL_SERIAL_H

#include "../kernel/types.h"
#include "../asm/asm.h" 

#define COM1 0x3F8 // COM1 base address - the first serial port

#define serial_write_format(fmt, ...) \
    do { \
        uint32_t args[] = { __VA_ARGS__ }; \
        serial_write_format_impl(fmt, args, sizeof(args) / sizeof(args[0])); \
    } while (0)

void serial_write(char c);
void serial_print(const char* str);
void serial_write_int(int num);
void serial_write_hex(int num);
void serial_write_format_impl(const char *fmt, uint32_t *args, int arg_count);

#endif // SERIAL_SERIAL_H