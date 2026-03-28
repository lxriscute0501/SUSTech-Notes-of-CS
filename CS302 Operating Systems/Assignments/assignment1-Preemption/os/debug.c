#include "debug.h"

#include "defs.h"

void print_trapframe(struct trapframe *tf) {
    printf("trapframe at %p\n", tf);
    printf("ra: %p  sp: %p  gp: %p  tp: %p\n", tf->ra, tf->sp, tf->gp, tf->tp);
    printf("t0: %p  t1: %p  t2: %p  s0: %p\n", tf->t0, tf->t1, tf->t2, tf->s0);
    printf("s1: %p  a0: %p  a1: %p  a2: %p\n", tf->s1, tf->a0, tf->a1, tf->a2);
    printf("a3: %p  a4: %p  a5: %p  a6: %p\n", tf->a3, tf->a4, tf->a5, tf->a6);
    printf("a7: %p  s2: %p  s3: %p  s4: %p\n", tf->a7, tf->s2, tf->s3, tf->s4);
    printf("s5: %p  s6: %p  s7: %p  s8: %p\n", tf->s5, tf->s6, tf->s7, tf->s8);
    printf("s9: %p s10: %p s11: %p  t3: %p\n", tf->s9, tf->s10, tf->s11, tf->t3);
    printf("t4: %p  t5: %p  t6: %p  \n\n", tf->t4, tf->t5, tf->t6);
}

void print_ktrapframe(struct ktrapframe *tf) {
    printf("kernel trapframe at %p\n", tf);
    printf("ra: %p   sp: %p   gp: %p  tp: %p\n", tf->ra, tf->sp, tf->gp, tf->tp);
    printf("t0: %p   t1: %p   t2: %p  s0: %p\n", tf->t0, tf->t1, tf->t2, tf->s0);
    printf("s1: %p   a0: %p   a1: %p  a2: %p\n", tf->s1, tf->a0, tf->a1, tf->a2);
    printf("a3: %p   a4: %p   a5: %p  a6: %p\n", tf->a3, tf->a4, tf->a5, tf->a6);
    printf("a7: %p   s2: %p   s3: %p  s4: %p\n", tf->a7, tf->s2, tf->s3, tf->s4);
    printf("s5: %p   s6: %p   s7: %p  s8: %p\n", tf->s5, tf->s6, tf->s7, tf->s8);
    printf("s9: %p  s10: %p  s11: %p  t3: %p\n", tf->s9, tf->s10, tf->s11, tf->t3);
    printf("t4: %p   t5: %p   t6: %p  \n\n", tf->t4, tf->t5, tf->t6);
}

void print_procs() {
    extern struct proc *pool[];

    for (int i = 0; i < NPROC; i++) {
        struct proc *p = pool[i];
        if (p->state == UNUSED)
            continue;
        printf("proc %d: %p\n", i, p);
        printf("  pid: %d, state: %d\n", p->pid, p->state);
//        printf("  mm: %p\n", p->mm);
        printf("  parent: %p", p->parent);
        if (p->parent)
            printf(" pid: %d", p->parent->pid);
        printf("\n");
    }
}
void print_kpgmgr() {
    extern int64 freepages_count;
    printf("freepages_count: %d\n", freepages_count);
}

void print_sysregs(int explain) {
    uint64 sstatus = r_sstatus();
    uint64 scause  = r_scause();
    uint64 sie     = r_sie();
    uint64 sepc    = r_sepc();
    uint64 stval   = r_stval();
    uint64 sip     = r_sip();
    uint64 satp    = r_satp();
    printf("sstatus : %p\n", sstatus);
    if (explain)
        printf("- SUM:%d, SPP:%c, SPIE:%d, SIE: %d\n",
               (sstatus & SSTATUS_SUM) != 0,
               ((sstatus & SSTATUS_SPP) ? 'S' : 'U'),
               (sstatus & SSTATUS_SPIE) != 0,
               (sstatus & SSTATUS_SIE) != 0);
    printf("scause  : %p\n", scause);
    if (explain)
        printf("- Interrupt:%d, Code:%d\n", (scause & SCAUSE_INTERRUPT) != 0, (scause & SCAUSE_EXCEPTION_CODE_MASK));

    printf("sepc    : %p\n", sepc);
    printf("stval   : %p\n", stval);
    printf("sip     : %p\n", sip);
    if (explain)
        printf("- Pending: Software:%d, Timer:%d, External:%d\n", (sip & SIE_SSIE) != 0, (sip & SIE_STIE) != 0, (sip & SIE_SEIE) != 0);

    printf("sie     : %p\n", sie);
    if (explain)
        printf("- Enabled: Software:%d, Timer:%d, External:%d\n", (sie & SIE_SSIE) != 0, (sie & SIE_STIE) != 0, (sie & SIE_SEIE) != 0);

    printf("satp    : %p\n", satp);
}
