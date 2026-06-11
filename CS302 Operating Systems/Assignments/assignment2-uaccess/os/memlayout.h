#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H

#include "riscv.h"
#include "types.h"

// Kernel Memory Layout:

#define RISCV_DDR_BASE      0x80000000ull
#define VALID_PHYS_ADDR(pa) (((pa) >= KERNEL_PHYS_BASE && (pa) <= RISCV_DDR_BASE + PHYS_MEM_SIZE))

/**
 * Kernel Memory Layout:
 *
 * [0x0000_003f_ffff_f000] : Trampoline
 *
 * [0xffff_ffc0_0000_0000] : Kernel Direct Mapping of all physical pages (offseted by macro KVA_TO_PA & PA_TO_KVA)
 * 		Example: Phy addr 0x8040_0000 is mapped to 0xffff_ffc0_8040_0000, these mappings used 2MiB PTE.
 *
 * [0xffff_fffd_0000_0000] : Kernel Heap for fixed-size object allocations
 *
 * [0xffff_ffff_8020_0000] : Kernel Image
 * 		Example:
 * 				.text:	 		 [0xffff_ffff_8020_0000, 0xffff_ffff_8020_5000)
 * 				 	is mapped to [0x0000_0000_8020_0000, 0x0000_0000_8020_5000)
 * 				.rodata:
 *
 * [0xffff_ffff_a000_0000] : Device MMIO.
 *
 * [0xffff_ffff_d000_0000] : Kernel stack for processs.
 *
 * [0xffff_ffff_ff00_0000] : Kernel stack for scheduler.
 */

#define KERNEL_VIRT_BASE           0xffffffff80200000ull
#define KERNEL_PHYS_BASE           0x80200000ull
#define KERNEL_OFFSET              ((uint64)(KERNEL_VIRT_BASE - KERNEL_PHYS_BASE))
#define KERNEL_DIRECT_MAPPING_BASE 0xffffffc000000000ull
#define KERNEL_ALLOCATOR_BASE      0xfffffffd00000000ull
#define KERNEL_ALLOCATOR_GAP       0x0000000001000000ull

#define KERNEL_STACK_SCHED 0xffffffffff000000ull
#define KERNEL_STACK_PROCS 0xfffffffe00000000ull
#define KERNEL_STACK_SIZE  (2 * PGSIZE)

#define KERNEL_DEVICE_MMIO_BASE 0xffffffffd0000000ull
#define KERNEL_PLIC_BASE        (KERNEL_DEVICE_MMIO_BASE)
#define KERNEL_PLIC_SIZE        (0x4000000)
#define KERNEL_UART0_BASE       (KERNEL_DEVICE_MMIO_BASE + KERNEL_PLIC_SIZE)
#define KERNEL_UART0_SIZE       (PGSIZE)

// Kernel Memory Layout Ends.

// Kernel Device MMIO defines: (for QEMU targets)

#define UART0_PHYS 0x10000000L
#define PLIC_PHYS  0x0c000000L

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
#define USERVM_TOP (TRAPFRAME)
#define MAX_USERVA (TRAPFRAME - 1)


#endif  // MEMLAYOUT_H