# sys

- [sys](#sys)
  - [`vgarainbow`](#vgarainbow)


## `vgarainbow`

VGA Rainbow is a small bare-metal program created to display *Hello World!* in all colors provided by [VGA text mode](https://en.wikipedia.org/wiki/VGA_text_mode).

<video controls>
  <source src="./pub/vgarainbow.webm" type="video/webm">
  Your browser does not support the video tag.
</video>

This is an exetension of `hello-os` project, I added a simple [virtual screen](./vgarainbow/base/virtscr.h) and [keyboard controller](./vgarainbow/base/kbd.h) which can be further extended to create more complex programs.