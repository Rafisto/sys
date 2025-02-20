#define NULL 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// stack

extern uint32_t stack_begin;
extern uint32_t stack_end;

// linker symbols

// text - code
extern uint32_t start_text;
extern uint32_t end_text;

// data - initialized data
extern uint32_t start_data;
extern uint32_t end_data;

// bss - block started by symbol - uninitialized data
extern uint32_t start_bss;
extern uint32_t end_bss;
