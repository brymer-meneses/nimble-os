
# NimbleOS

> **Nimble**\
> *adjective*\
> Quick and light in movement in action; agile

NimbleOS, is a work in progress operating system made from scratch using C++, 
it is an attempt to understand the inner workings of an operating system, by
making one from scratch.


## Progress

- [x] Interrupts
- [x] Graphics
    - [x] Print strings to the screen 
    - [ ] Window Management
- [x] Custom Testing Framework (inspired by GTest)
- [ ] Serial Output
- [ ] Robust Logger
- [ ] Use APIC instead of PIC
- [x] Keyboard Support
- [x] Memory Management
    - [x] Physical Memory Manager (Bitmap for now)
    - [x] Virtual Memory Manager (Bump Allocator for now)
    - [x] Heap Allocation (`malloc` and friends)
        - [ ] Implement Block Coalescing
- [ ] Kernel STL
    - [x] Vector
    - [x] Linked List
    - [ ] Spin Mutex
- [ ] Scheduler
- [ ] Virtual File System
    - [ ] Ext2
- [ ] NVMe Driver
- [ ] Basic Shell
- [ ] Ports
    - [ ] Doom
    - [ ] GCC
    - [ ] Cargo
    - [ ] Vim

## Building

Currently, this project can only be built using `clang++` with version `>= 16.0.6`.

```bash
make <run|build> MODE=<debug|test|release>
```

Invoking `make`, will default to 
```bash
make run MODE=debug
```
