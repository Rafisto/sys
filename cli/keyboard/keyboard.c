#include "keyboard.h"
#include "../asm/asm.h"

#include "../console/console.h"

void keyboard_init(console_input_function func)
{
  console_buffer_function = func;

  unsigned char curmask_master = asm_read_port(0x21);
  asm_write_port(0x21, curmask_master & 0xFD);
}

void keyboard_handler(void)
{
  signed char keycode;

  keycode = asm_read_port(0x60);

  if (keycode >= 0 && keyboard_map[keycode])
  {
    console_buffer_function(keyboard_map[keycode]);
  }

  asm_write_port(0x20, 0x20);
}