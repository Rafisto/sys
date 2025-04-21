#ifndef DEV_MOUSE_H
#define DEV_MOUSE_H

#include "../../kernel/types.h"
#include "../../kernel/x86.h"

#define PS2_DATA        0x60
#define PS2_STATUS      0x64
#define PS2_COMMAND     0x64

#define MOUSE_ENABLE    0xF4
#define MOUSE_WRITE     0xD4

// Wait until we can write to controller
static void mouse_wait_write() {
    while (inb(PS2_STATUS) & 0x02);
}

// Wait until we can read from controller
static void mouse_wait_read() {
    while (!(inb(PS2_STATUS) & 0x01));
}

// Send a command to the mouse
static void mouse_write(uint8_t cmd) {
    mouse_wait_write();
    outb(PS2_COMMAND, MOUSE_WRITE); // Tell controller we want to talk to the mouse
    mouse_wait_write();
    outb(PS2_DATA, cmd);
}

// Read a byte from the mouse
static uint8_t mouse_read() {
    mouse_wait_read();
    return inb(PS2_DATA);
}

// Enable mouse device
void mouse_init() {
    mouse_write(MOUSE_ENABLE);
    mouse_read(); // ACK (should be 0xFA)
}

// Poll one 3-byte packet from mouse
typedef struct {
    int8_t dx;
    int8_t dy;
    uint8_t buttons;
} mouse_packet;

int mouse_poll(mouse_packet* packet) {
    if (!(inb(PS2_STATUS) & 0x01)) return 0; // no data ready

    uint8_t status = mouse_read();
    if (!(status & 0x08)) return 0; // not a valid packet

    packet->buttons = status & 0x07;

    mouse_wait_read();
    packet->dx = (int8_t)mouse_read();

    mouse_wait_read();
    packet->dy = (int8_t)mouse_read();

    return 1;
}

#endif // DEV_MOUSE_H