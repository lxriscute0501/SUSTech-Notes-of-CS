#ifndef RISCV_H
#define RISCV_H

#include "types.h"

// make the vscode language server happy
#define asm __asm__

// Supervisor Status Register, sstatus
#define SSTATUS_SUM  (1L << 18)  // SUM (permit Supervisor User Memory access)
#define SSTATUS_SPP  (1L << 8)   // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5)   // Supervisor Previous Interrupt Enable
#define SSTATUS_SIE  (1L << 1)   // Supervisor Interrupt Enable

static inline uint64 r_sstatus() {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r"(x));
    return x;
}

static inline void w_sstatus(uint64 x) {
    asm volatile("csrw sstatus, %0" : : "r"(x));
}

// Supervisor Interrupt Pending
static inline uint64 r_sip() {
    uint64 x;
    asm volatile("csrr %0, sip" : "=r"(x));
    return x;
}

static inline void w_sip(uint64 x) {
    asm volatile("csrw sip, %0" : : "r"(x));
}

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9)  // external
#define SIE_STIE (1L << 5)  // timer
#define SIE_SSIE (1L << 1)  // software
static inline uint64 r_sie() {
    uint64 x;
    asm volatile("csrr %0, sie" : "=r"(x));
    return x;
}

static inline void w_sie(uint64 x) {
    asm volatile("csrw sie, %0" : : "r"(x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void w_sepc(uint64 x) {
    asm volatile("csrw sepc, %0" : : "r"(x));
}

static inline uint64 r_sepc() {
    uint64 x;
    asm volatile("csrr %0, sepc" : "=r"(x));
    return x;
}

// Supervisor Trap-Vector Base Address
// low two bits are mode.
static inline void w_stvec(uint64 x) {
    asm volatile("csrw stvec, %0" : : "r"(x));
}

static inline uint64 r_stvec() {
    uint64 x;
    asm volatile("csrr %0, stvec" : "=r"(x));
    return x;
}

// use riscv's sv39 page table scheme.
#define SATP_SV39 (8L << 60)

#define MAKE_SATP(pagetable)  (SATP_SV39 | (((uint64)pagetable) >> 12))
#define SATP_TO_PGTABLE(satp) ((pagetable_t)(((satp) & ((1ULL << 44) - 1)) << PGSHIFT))

// supervisor address translation and protection;
// holds the address of the page table.
static inline void w_satp(uint64 x) {
    // NOMMU:
    extern __attribute__((noreturn)) void __panic(char *fmt, ...);
    __panic("nommu mode");
    asm volatile("csrw satp, %0" : : "r"(x));
}

static inline uint64 r_satp() {
    uint64 x;
    asm volatile("csrr %0, satp" : "=r"(x));
    return x;
}

// Supervisor Scratch register, for early trap handler in trampoline.S.
static inline void w_sscratch(uint64 x) {
    asm volatile("csrw sscratch, %0" : : "r"(x));
}

static inline void w_mscratch(uint64 x) {
    asm volatile("csrw mscratch, %0" : : "r"(x));
}

// Supervisor Trap Cause
static inline uint64 r_scause() {
    uint64 x;
    asm volatile("csrr %0, scause" : "=r"(x));
    return x;
}

// Supervisor Trap Value
static inline uint64 r_stval() {
    uint64 x;
    asm volatile("csrr %0, stval" : "=r"(x));
    return x;
}

// machine-mode cycle counter
static inline uint64 r_time() {
    uint64 x;
    asm volatile("csrr %0, time" : "=r"(x));
    return x;
}

// enable device interrupts
static inline void intr_on() {
    // set SIE bit
    asm volatile("csrrs x0, sstatus, %0" ::"r"(SSTATUS_SIE));
    // w_sstatus(r_sstatus() | SSTATUS_SIE);
}

// disable device interrupts, return whether it opens before off.
static inline int64 intr_off() {
    uint64 prev;
    asm volatile("csrrc %0, sstatus, %1" : "=r"(prev) : "r"(SSTATUS_SIE));
    return (prev & SSTATUS_SIE) != 0;
    // w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

// are device interrupts enabled?
static inline int intr_get() {
    uint64 x = r_sstatus();
    return (x & SSTATUS_SIE) != 0;
}

static inline uint64 r_sp() {
    uint64 x;
    asm volatile("mv %0, sp" : "=r"(x));
    return x;
}

// read and write tp, the thread pointer, which holds
// this core's hartid (core number), the index into cpus[].
static inline uint64 r_tp() {
    uint64 x;
    asm volatile("mv %0, tp" : "=r"(x));
    return x;
}

static inline void w_tp(uint64 x) {
    asm volatile("mv tp, %0" : : "r"(x));
}

static inline uint64 r_ra() {
    uint64 x;
    asm volatile("mv %0, ra" : "=r"(x));
    return x;
}

static inline uint64 r_pc() {
    uint64 pc;
    asm volatile("auipc %0, 0" : "=r"(pc));
    return pc;
}

// flush the TLB.
static inline void sfence_vma() {
    // the zero, zero means flush all TLB entries.
    asm volatile("sfence.vma zero, zero");
}

#define PGSIZE    4096      // bytes per page
#define PGSIZE_2M 0x200000  // bytes per page
#define PGSHIFT   12        // bits of offset within a page

#define ROUNDUP_2N(sz, base) (((sz) + (base) - 1) & ~((base) - 1))
#define IS_ALIGNED(a, base)  (((a) & ((base) - 1)) == 0)

#define PGROUNDUP(sz)  (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))
#define PGALIGNED(a)   (((a) & (PGSIZE - 1)) == 0)

#define PTE_V (1L << 0)  // valid
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)  // 1 -> user can access
#define PTE_G (1L << 5)
#define PTE_A (1L << 6)
#define PTE_D (1L << 7)

#define PTE_RWX (PTE_R | PTE_W | PTE_X)

// shift a physical address to the right place for a PTE.
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

#define PTE2PA(pte) (((pte) >> 10) << 12)

#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// extract the three 9-bit page table indices from a virtual address.
#define PXMASK         0x1FF  // 9 bits
#define PXSHIFT(level) (PGSHIFT + (9 * (level)))
#define PX(level, va)  ((((uint64)(va)) >> PXSHIFT(level)) & PXMASK)

#define MAKE_PTE(pa, flags) (PA2PTE(pa) | (flags | PTE_V))

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv39, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

typedef uint64 pte_t;
typedef uint64 pde_t;
typedef uint64 *pagetable_t;  // 512 PTEs

#endif  // RISCV_H