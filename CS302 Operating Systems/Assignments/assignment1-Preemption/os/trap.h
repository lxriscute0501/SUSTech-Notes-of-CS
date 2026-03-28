#ifndef TRAP_H
#define TRAP_H

#include "types.h"

#define SCAUSE_INTERRUPT           ((1ull) << 63)
#define SCAUSE_EXCEPTION_CODE_MASK (((1ull) << 63) - 1)

/**
 * @brief Kernel trap frame
 *
 * Used when we are in the kernel, and we meet an Exception or Interrupt to get into the trap.acquire
 *
 * This ktrapframe is stored in the kernel stack.
 *
 */
struct ktrapframe {
    uint64 x0;  // x0
    uint64 ra;
    uint64 sp;
    uint64 gp;
    uint64 tp;
    uint64 t0;
    uint64 t1;
    uint64 t2;
    uint64 s0;
    uint64 s1;
    uint64 a0;
    uint64 a1;
    uint64 a2;
    uint64 a3;
    uint64 a4;
    uint64 a5;
    uint64 a6;
    uint64 a7;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
    uint64 t3;
    uint64 t4;
    uint64 t5;
    uint64 t6;

    // 32 * 8 bytes = 256 (0x100) bytes
};

/**
 * @brief User-mode Trap frame
 *
 * Used when we were in the user-mode, and we meet an Exception (including ecall) or Interrupt to get into the trap.
 * 
 * This trapframe is stored in the special physical page, which is mapped to the same VA `TRAPFRAME`.
 *
 */
struct trapframe {
    /*   0 */ uint64 kernel_satp;    // kernel page table
    /*   8 */ uint64 kernel_sp;      // top of process's kernel stack
    /*  16 */ uint64 kernel_trap;    // usertrap()
    /*  24 */ uint64 epc;            // saved user program counter
    /*  32 */ uint64 kernel_hartid;  // saved kernel tp
    /*  40 */ uint64 ra;
    /*  48 */ uint64 sp;
    /*  56 */ uint64 gp;
    /*  64 */ uint64 tp;
    /*  72 */ uint64 t0;
    /*  80 */ uint64 t1;
    /*  88 */ uint64 t2;
    /*  96 */ uint64 s0;
    /* 104 */ uint64 s1;
    /* 112 */ uint64 a0;
    /* 120 */ uint64 a1;
    /* 128 */ uint64 a2;
    /* 136 */ uint64 a3;
    /* 144 */ uint64 a4;
    /* 152 */ uint64 a5;
    /* 160 */ uint64 a6;
    /* 168 */ uint64 a7;
    /* 176 */ uint64 s2;
    /* 184 */ uint64 s3;
    /* 192 */ uint64 s4;
    /* 200 */ uint64 s5;
    /* 208 */ uint64 s6;
    /* 216 */ uint64 s7;
    /* 224 */ uint64 s8;
    /* 232 */ uint64 s9;
    /* 240 */ uint64 s10;
    /* 248 */ uint64 s11;
    /* 256 */ uint64 t3;
    /* 264 */ uint64 t4;
    /* 272 */ uint64 t5;
    /* 280 */ uint64 t6;
};

enum Exception {
    InstructionMisaligned  = 0,
    InstructionAccessFault = 1,
    IllegalInstruction     = 2,
    Breakpoint             = 3,
    LoadMisaligned         = 4,
    LoadAccessFault        = 5,
    StoreMisaligned        = 6,
    StoreAccessFault       = 7,
    UserEnvCall            = 8,
    SupervisorEnvCall      = 9,
    MachineEnvCall         = 11,
    InstructionPageFault   = 12,
    LoadPageFault          = 13,
    StorePageFault         = 15,
};

enum Interrupt {
    UserSoft = 0,
    SupervisorSoft,
    UserTimer = 4,
    SupervisorTimer,
    UserExternal = 8,
    SupervisorExternal,
};

void trap_init();
void kerneltrap(struct ktrapframe *ktf);
void usertrapret();

#endif  // TRAP_H