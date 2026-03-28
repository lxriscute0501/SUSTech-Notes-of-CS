#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H

#include "riscv.h"
#include "types.h"

// Kernel Memory Layout:

#define RISCV_DDR_BASE      0x80000000ull
#define VALID_PHYS_ADDR(pa) (((pa) >= KERNEL_PHYS_BASE && (pa) <= RISCV_DDR_BASE + PHYS_MEM_SIZE))

#define KERNEL_PHYS_BASE 0x80200000ull

// Kernel Memory Layout Ends.

// Kernel Device MMIO defines: (for QEMU targets)

#define UART0_PHYS 0x10000000L
#define PLIC_PHYS  0x0c000000L

#define KERNEL_UART0_BASE UART0_PHYS
#define KERNEL_PLIC_BASE  PLIC_PHYS

// User Memory Layout:

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv39, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

// map the trampoline page to the highest address,
// in both user and kernel space.
#define USER_TOP   (MAXVA)
#define TRAMPOLINE (USER_TOP - PGSIZE)
#define TRAPFRAME  (TRAMPOLINE - PGSIZE)

#endif  // MEMLAYOUT_H