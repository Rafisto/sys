#ifndef SERIAL_UART_H
#define SERIAL_UART_H

#include "../kernel/types.h"
#include "../kernel/x86.h"

#define slog(fmt, ...)             \
    do                             \
    {                              \
        sprint("[");               \
        sprint(__FILE__);          \
        sprint(":");               \
        sprinti(__LINE__);         \
        sprint("] ");              \
        sprintf(fmt, __VA_ARGS__); \
        sprint("\n");              \
    } while (0)

#define sprintf(fmt, ...)                                        \
    do                                                           \
    {                                                            \
        uint32_t args[] = {__VA_ARGS__};                         \
        sprintf_impl(fmt, args, sizeof(args) / sizeof(args[0])); \
    } while (0)

void sprint(const char *str);
void sprintc(char c);
void sprinti(int num);
void sprinth(int num);

void sprintf_impl(const char *fmt, uint32_t *args, int arg_count);

#endif // SERIAL_SERIAL_H