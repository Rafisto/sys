# Set default architecture for GDB (in case you're debugging 32-bit x86)
set architecture i386

# Set default file path for the kernel ELF binary
file _build/kernel.elf

# Automatically load symbols for the kernel
target remote localhost:1234

# Set breakpoints
break _start
break main                   # Add any other function breakpoints here

# Automatically start running once GDB is attached
run

# Show disassembly for better insight if needed
set disassembly-flavor intel

# Set GDB to be verbose with errors and warnings
set verbose on

# Make sure GDB uses the correct debugging symbols
set print pretty on
