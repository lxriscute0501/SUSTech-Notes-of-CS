#ifndef DEFS_H
#define DEFS_H

// defs.h contains most common definitions and declarations.

// ask clang-format do not sort the includes
// clang-format off

#include "types.h"
#include "riscv.h"
#include "log.h"
#include "memlayout.h"
#include "string.h"
#include "vm.h"
#include "proc.h"
#include "lock.h"
#include "kalloc.h"

// clang-format on

// Kernel defines
#define ENABLE_SMP    (1)
#define NCPU          (4)
#define NPROC         (512)
#define KSTRING_MAX   (256)
#define MAXARG        (32)
#define PHYS_MEM_SIZE (64ull * 1024 * 1024)

// Common macros
#define MIN(a, b)      (a < b ? a : b)
#define MAX(a, b)      (a > b ? a : b)
#define MEMORY_FENCE() __sync_synchronize()
#define __noreturn     __attribute__((noreturn))

// Kernel string buffer
extern allocator_t kstrbuf;

// kernel image symbols, defined in kernel.ld
extern char skernel[], ekernel[];
extern char s_rodata[], e_rodata[];
extern char s_text[], e_text[];
extern char s_data[], e_data[];
extern char s_bss[], e_bss[];

// entry.S
extern char _entry[];
extern char _entry_secondary_cpu[];
extern char kernel_trap_entry[];
extern char boot_stack[], boot_stack_top[];

// trampoline.S
extern char trampoline[];
extern char uservec[];
extern char userret[];

#endif  // DEFS_H