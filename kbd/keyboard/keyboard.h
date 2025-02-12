#ifndef BASE_KEYBOARD_H
#define BASE_KEYBOARD_H

#include "../asm/asm.h"

#define SHIFT (1 << 0)
#define CTL (1 << 1)
#define ALT (1 << 2)
#define CAPSLOCK (1 << 3)
#define NUMLOCK (1 << 4)
#define SCROLLLOCK (1 << 5)
#define E0ESC (1 << 6)
#define KEY_HOME 0xE0
#define KEY_END 0xE1
#define KEY_UP 0xE2
#define KEY_DN 0xE3
#define KEY_LF 0xE4
#define KEY_RT 0xE5
#define KEY_PGUP 0xE6
#define KEY_PGDN 0xE7
#define KEY_INS 0xE8
#define KEY_DEL 0xE9

static unsigned char keyboard_map[256] = {
    0, 0x1B, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0,
    [0x9C] '\n',
    [0xB5] '/',
    [0xC8] KEY_UP, [0xD0] KEY_DN,
    [0xC9] KEY_PGUP, [0xD1] KEY_PGDN,
    [0xCB] KEY_LF, [0xCD] KEY_RT,
    [0x97] KEY_HOME, [0xCF] KEY_END,
    [0xD2] KEY_INS, [0xD3] KEY_DEL
};

typedef void (*console_input_function)(char);
static console_input_function console_buffer_function;

void keyboard_init(console_input_function func);
void keyboard_handler(void);

#endif // BASE_KEYBOARD_H
