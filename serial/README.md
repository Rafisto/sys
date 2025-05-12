## `misc/serial`

We found a serial connection on a weird embedded device. The device is running a custom firmware that we don't have the source code for. The firmware is a bit weird, and it doesn't seem to be doing anything useful. 

```bash
nc localhost 4000
```

(local setup using `make qemu` in challenge directory)