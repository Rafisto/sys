## RafistOS

- [RafistOS](#rafistos)
  - [Boot Sequence](#boot-sequence)
  - [Goals](#goals)
  - [Paging](#paging)

### Boot Sequence

- Load kernel
- Setup GDT
- Setup Stack
- Process Multiboot2
- Setup Predefined Memory Map
- Setup Paging
- Begin `kmain()`

### Goals

- Implement Generic Interrupt Handling
- Implement APIC/PIT
- Implement Context Switches and Process Management
- Create a GUI Library

### Paging

In order to enable paging at boot I prepare a few things in [boot.asm](./boot/boot.asm)
- Enable [Page Size Extension](https://en.wikipedia.org/wiki/Page_Size_Extension) - `CR4_PAGE_SIZE_EXTENSION 0x00000010` - allow pages greater than 4KiB in size.
  - 4 MiB coexist with 4KiB pages, however if PAE is enabled large page size is reduced to 2 MiB. I am currently not using PAE.
- Enable [32-bit Paging](https://wiki.osdev.org/Paging)
  - Must move first page directory to `CR3`
  - Then set `CR0_PAGING 0x80000001` (31-st bit) to enable paging 

Let's use Linux Higher Half Kernel `0xC0000000 – 0xFFFFFFFF`, because it is a common practice.