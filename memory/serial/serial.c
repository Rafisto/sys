#include "serial.h"

void serial_write(char c) {
    while ((inb(COM1 + 5) & 0x20) == 0);
    outb(COM1, c);
}

void serial_print(const char* str) {
    while (*str) {
        serial_write(*str++);
    }
}

void serial_write_int(int num) {
    char buf[12];
    int i = 0;
    if (num < 0) {
        serial_write('-');
        num = -num;
    }
    if (num == 0) {
        serial_write('0');
        return;
    }
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i--) {
        serial_write(buf[i]);
    }
}

void serial_write_hex(int num) {
    for (int i = 28; i >= 0; i -= 4) {
        int digit = (num >> i) & 0xF;
        if (digit < 10)
            serial_write('0' + digit);
        else
            serial_write('a' + digit - 10);
    }
}

void serial_write_format_impl(const char *fmt, uint32_t *args, int arg_count) {
    int arg_index = 0;
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'd':
                    if (arg_index < arg_count) serial_write_int(args[arg_index++]);
                    break;
                case 'x':
                    serial_write('0');
                    serial_write('x');
                    if (arg_index < arg_count) serial_write_hex(args[arg_index++]);
                    break;
                default:
                    serial_write('%');
                    serial_write(*fmt);
                    break;
            }
        } else {
            serial_write(*fmt);
        }
        fmt++;
    }
}

void serial_read(char *buffer, int size) {
    for (int i = 0; i < size; i++) {
        while ((inb(COM1 + 5) & 0x01) == 0);
        buffer[i] = inb(COM1);
    }
    serial_write('\n');
    serial_print(buffer);
    serial_write('\n');
}