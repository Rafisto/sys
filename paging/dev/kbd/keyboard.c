#include "keyboard.h"

void keyboard_init()
{
  unsigned char curmask_master = inb(0x21);
  outb(0x21, curmask_master & 0xFD);
}

void keyboard_handler(void)
{
  signed char keycode;

  keycode = inb(0x60);

  if (keycode >= 0 && keyboard_map[keycode])
  {
    fb_sprintc(keyboard_map[keycode]);
  }

  outb(0x20, 0x20);
}