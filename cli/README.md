```bash
make qemu-gdb
gdb -ex "target remote localhost:1234" -ex "symbol-file _build/kernel.elf"
```