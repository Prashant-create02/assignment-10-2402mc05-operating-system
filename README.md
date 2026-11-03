# Operating Systems — Assignment 10
## Memory Management, Address Translation, Page Protection, and Page Replacement in xv6

### 1. Overview
This project extends the MIT `xv6-public` (x86) operating system kernel to implement physical memory page tracking, virtual-to-physical address translation, page-table inspection/protection mechanisms, and a user-space simulation comparing FIFO, LRU, and Optimal page replacement algorithms.

---

### 2. Modified & Added Files

#### A. Modified Kernel Files
| File | Description of Modifications |
| :--- | :--- |
| `syscall.h` | Defined syscall numbers for `SYS_memstats` (22), `SYS_vtop` (23), `SYS_pageinfo` (24), and `SYS_setreadonly` (25). |
| `syscall.c` | Added `extern` declarations and registered `sys_memstats`, `sys_vtop`, `sys_pageinfo`, and `sys_setreadonly` in `syscalls[]`. |
| `user.h` | Added user-space system call prototypes for `memstats()`, `vtop()`, `pageinfo()`, and `setreadonly()`. |
| `usys.S` | Added assembly `SYSCALL` trap stubs for the four new system calls. |
| `defs.h` | Declared kernel helper functions `memstats_info()`, `vtop_translate()`, `pageinfo_inspect()`, and `page_setreadonly()`. |
| `kalloc.c` | Extended `kmem` struct with `total_allocated`, `total_freed`, `available_pages`, and `in_freerange`; updated `kinit1()`, `freerange()`, `kalloc()`, and `kfree()`, and implemented `memstats_info()`. |
| `vm.c` | Implemented `vtop_translate()`, `pageinfo_inspect()`, and `page_setreadonly()` using `walkpgdir()` and `lcr3()`. |
| `sysproc.c` | Implemented syscall wrapper handlers `sys_memstats()`, `sys_vtop()`, `sys_pageinfo()`, and `sys_setreadonly()`. |
| `Makefile` | Added `_memtest`, `_vttest`, `_pagetest`, and `_pagereplace` to `UPROGS` and `EXTRA`. |

#### B. Added User Test Programs
* `memtest.c` — Tests `memstats()` before and after allocating/deallocating 5 pages via `sbrk()`.
* `vttest.c` — Tests `vtop()` across code addresses, heap-allocated pages (with offsets), and unmapped virtual addresses.
* `pagetest.c` — Tests `pageinfo()` and `setreadonly()`, demonstrating a hardware Page Fault (`trap 14`) upon writing to a read-only page.
* `pagereplace.c` — Implements and compares **FIFO**, **LRU**, and **Optimal** page replacement algorithms across 3, 4, and 5 frames.

---

### 3. Build & Execution Instructions

1. **Clean previous build artifacts and compile xv6:**
   ```bash
   make clean
   make qemu-nox
   ```
