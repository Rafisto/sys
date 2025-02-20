Table of Contents

- [Command Line Interface](#command-line-interface) - CLI with a few commands to interact with the system.
- [Keyboard on Interrupts](#keyboard-on-interrupts) - Keyboard controller on x86 with GDT, IDT and IRQ.
- [VGA Rainbow](#vga-rainbow) - Display *Hello World!* in all colors provided by VGA text mode.
 
## Command Line Interface

CLI presents a few commands to interact with the system. It allows for specifying up to 16 arguments for each command. Just by adding a new function to the `commands.c` and updating the `commands.h` file you can add a new command.

![Command Line Interface](./pub/cli.gif)

```c
void sum_command(const int argc, const char const* const* argv) {
    int sum = 0;
    for (int i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    write_format("%d\n", sum);
}
```

I also added simple `kalloc()` and set up paging to be used later for dynamic memory allocation. 

## Keyboard on Interrupts

KBD is a continuation of my bare-metal journey. Thanks to this [stackoverflow answer](https://stackoverflow.com/a/37635449) I was able to successfully recreate a keyboard controller on interrupts.

![Keyboard on Interrupts](./pub/kbd.gif)

Most of the program is done in [boot.asm](./kbd/boot.asm), where I initialize GDT, and in [idt.h](./kbd/idt/idt.h), where I use the interrupt controller. A little dependency injection was made in the `console.c`, as the `console_input()` is actually passed as a function to be called after a key is pressed.

## VGA Rainbow

VGA Rainbow is a small bare-metal program created to display *Hello World!* in all colors provided by [VGA text mode](https://en.wikipedia.org/wiki/VGA_text_mode).

![VGA Rainbow Showcase GIF](./pub/vgarainbow.gif)

This is an exetension of `hello-os` project, I added a simple [virtual screen](./vgarainbow/base/virtscr.h) and [keyboard controller](./vgarainbow/base/kbd.h) which can be further extended to create more complex programs.