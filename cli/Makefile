CC = gcc
LD = ld
CDEBUG = -g
CFLAGS = -ffreestanding -nostdlib -nodefaultlibs -no-pie -fno-pic -static -fno-stack-protector -m32 -Iinclude $(CDEBUG)
LDFLAGS = -m elf_i386

kernel := _build/kernel.bin
kernel_elf := _build/kernel.elf
iso := _build/hello.iso

linker_script := linker.ld
grub_cfg := grub.cfg
assembly_source_files := multiboot_header.asm boot.asm $(wildcard asm/*.asm) $(wildcard paging/*.asm)
assembly_object_files := $(patsubst %.asm, _build/%.o, $(assembly_source_files))
c_source_dirs := $(shell find . -type d)
c_source_files := $(foreach dir, $(c_source_dirs), $(wildcard $(dir)/*.c))
c_object_files := $(patsubst %.c, _build/%.o, $(c_source_files))

.PHONY: all clean iso kernel qemu qemu-gdb

all: $(kernel) $(kernel_elf)

clean:
	- @rm -fr _build
	- @rm -f serial.log *.o
	- @rm -fr _build/isofiles

qemu: $(iso)
	qemu-system-x86_64 -cdrom $(iso) -vga std -s -serial file:serial.log -serial stdio

qemu-gdb: $(iso)
	qemu-system-x86_64 -cdrom $(iso) -vga std -s -serial file:serial.log -serial stdio -S

iso: $(iso)
	@echo "Done"

$(iso): $(kernel) $(grub_cfg)
	mkdir -p _build/isofiles/boot/grub
	cp $(kernel) _build/isofiles/boot/kernel.bin
	cp $(grub_cfg) _build/isofiles/boot/grub
	grub-mkrescue -o $(iso) _build/isofiles #2> /dev/null

$(kernel): $(c_object_files) $(assembly_object_files) $(linker_script)
	echo $(c_object_files)
	ld $(LDFLAGS) -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files)

$(kernel_elf): $(c_object_files) $(assembly_object_files) $(linker_script)
	echo $(c_object_files)
	ld $(LDFLAGS) -T $(linker_script) -o $(kernel_elf) $(assembly_object_files) $(c_object_files)

_build/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

# compile assembly files
_build/%.o: %.asm
	@mkdir -p $(shell dirname $@)
	nasm -felf32 $< -o $@
