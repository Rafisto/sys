#ifndef BASE_HANDLERS_H
#define BASE_HANDLERS_H

#include "../screen/fscreen.h"

void divzero_handler();
void print_registers();

void divzero_handler() {
    write_string("Division by zero exception\n");
    print_registers();
}

void print_registers() {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp, eip;

    asm volatile("mov %%eax, %0" : "=r"(eax));
    asm volatile("mov %%ebx, %0" : "=r"(ebx));
    asm volatile("mov %%ecx, %0" : "=r"(ecx));
    asm volatile("mov %%edx, %0" : "=r"(edx));
    asm volatile("mov %%esi, %0" : "=r"(esi));
    asm volatile("mov %%edi, %0" : "=r"(edi));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("call 1f; 1: pop %0" : "=r"(eip));

    write_string("Register dump:\n");
    write_string("EAX: "); write_int(eax); write_string("\n");
    write_string("EBX: "); write_int(ebx); write_string("\n");
    write_string("ECX: "); write_int(ecx); write_string("\n");
    write_string("EDX: "); write_int(edx); write_string("\n");
    write_string("ESI: "); write_int(esi); write_string("\n");
    write_string("EDI: "); write_int(edi); write_string("\n");
    write_string("EBP: "); write_int(ebp); write_string("\n");
    write_string("ESP: "); write_int(esp); write_string("\n");
    write_string("EIP: "); write_int(eip); write_string("\n");

    write_string("\n");
}

#endif // BASE_HANDLERS_H