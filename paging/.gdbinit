set architecture i386
file _build/kernel.elf
target remote localhost:1234

break _start
break kmain

set disassembly-flavor intel
set verbose on
set print pretty on

run
