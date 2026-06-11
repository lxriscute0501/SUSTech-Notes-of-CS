#include "trap.h"

#include "console.h"
#include "debug.h"
#include "defs.h"
#include "loader.h"
#include "plic.h"
#include "syscall.h"
#include "timer.h"

static int64 kp_print_lock = 0;
extern volatile int panicked;

struct spinlock tickslock;
uint64 ticks;

void plic_handle() {
    int irq = plic_claim();
    if (irq == uart0_irq) {
        uart_intr();
        // printf("intr %d: UART0\n", r_tp());
    }

    if (irq)
        plic_complete(irq);
}

static int handle_intr(void) {
    uint64 cause = r_scause();
    uint64 code  = cause & SCAUSE_EXCEPTION_CODE_MASK;
    if (code == SupervisorTimer) {
        tracef("time interrupt!");
        if (cpuid() == 0) {
            acquire(&tickslock);
            ticks++;
            wakeup(&ticks);
            release(&tickslock);
        }
        set_next_timer();
        return 1;
    } else if (code == SupervisorExternal) {
        tracef("s-external interrupt from usertrap!");
        plic_handle();
        return 2;
    } else {
        return 0;
    }
}

void kernel_trap(struct ktrapframe *ktf) {
    assert(!intr_get());

    if ((r_sstatus() & SSTATUS_SPP) == 0)
        panic("kerneltrap: not from supervisor mode");

    mycpu()->inkernel_trap++;

    uint64 cause          = r_scause();
    uint64 exception_code = cause & SCAUSE_EXCEPTION_CODE_MASK;
    if (cause & SCAUSE_INTERRUPT) {
        // correctness checking:
        if (mycpu()->inkernel_trap > 1) {
            // should never have nested interrupt
            print_sysregs(true);
            print_ktrapframe(ktf);
            panic("nested kerneltrap");
        }
        if (panicked) {
            panic("other CPU has panicked");
        }
        int whichdev = 0;
        // handle interrupt
        if ((whichdev = handle_intr()) == 0) {
            errorf("unhandled interrupt: %d", cause);
            goto kernel_panic;
        }
        if (whichdev == 1 && curr_proc() != NULL) {
            uint64 sepc, sstatus;
            sepc   = r_sepc();
            sstatus = r_sstatus();
            mycpu()->inkernel_trap--;
            // warnf("kernel thread preempt");
            yield();
            mycpu()->inkernel_trap++;
            w_sepc(sepc);
            w_sstatus(sstatus);
        }
    } else {
        // kernel exception, unexpected.
        goto kernel_panic;
    }

    assert(!intr_get());
    assert(mycpu()->inkernel_trap == 1);

    mycpu()->inkernel_trap--;

    return;

kernel_panic:
    // lock against other cpu, to show a complete panic message.
    panicked = 1;

    while (__sync_lock_test_and_set(&kp_print_lock, 1) != 0);

    errorf("=========== Kernel Panic ===========");
    print_sysregs(true);
    print_ktrapframe(ktf);

    __sync_lock_release(&kp_print_lock);

    panic("kernel panic");
}

void set_kerneltrap() {
    assert(IS_ALIGNED((uint64)kernel_trap_entry, 4));
    w_stvec((uint64)kernel_trap_entry);  // DIRECT
}

// set up to take exceptions and traps while in the kernel.
void trap_init() {
    set_kerneltrap();
    spinlock_init(&tickslock, "user-time");
}

// UserTrap begins

static void handle_pgfault(void) {
    uint64 cause   = r_scause();
    uint64 addr    = r_stval();
    struct proc *p = curr_proc();
    struct mm *mm;
    pte_t *pte;

    acquire(&p->lock);
    mm = p->mm;
    acquire(&mm->lock);
    release(&p->lock);
    pte = walk(mm, addr, 0);
    release(&mm->lock);

    //	docs: Volume II: RISC-V Privileged Architectures V1.10, Page 61,
    //		> Two schemes to manage the A and D bits are permitted:
    // 			- ..., the implementation(hardware) sets the corresponding bit in the PTE.
    //			- ..., a page-fault exception is raised.
    //		> Standard supervisor software should be written to assume either or both PTE update schemes may be in effect.

    if (pte != NULL && (*pte & PTE_V) && (*pte & PTE_U)) {
        if (!(*pte & PTE_A) || (cause == StorePageFault && !(*pte & PTE_D))) {
            // page fault possibly due to missing A/D bit
            // - Load/IF PageFault: Missing A bit
            // - Store PageFault  : Missing A/D bit
            *pte |= PTE_A;
            if (cause == StorePageFault)
                *pte |= PTE_D;    
            return;
        }
    }
    // otherwise, it is a page fault due to invalid address
    infof("page fault in application, bad addr = %p, bad instruction = %p, core dumped.", r_stval(), p->trapframe->epc);
    setkilled(p, -2);
}

static void unknown_trap(void) {
    print_sysregs(true);
    vm_print(curr_proc()->mm->pgt);
    errorf("unknown trap: %p, stval = %p", r_scause(), r_stval());
    setkilled(curr_proc(), -3);
}

// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
void usertrap() {
    set_kerneltrap();

    if (intr_get())
        panic("entered interrupts enabled");
    if ((r_sstatus() & SSTATUS_SPP) != 0)
        panic("usertrap: not from user mode");

    int which_dev               = 0;
    struct proc *p              = curr_proc();
    struct trapframe *trapframe = p->trapframe;
    tracef("trap from user epc = %p", trapframe->epc);
    int killed;

    uint64 cause = r_scause();
    if (cause & SCAUSE_INTERRUPT) {
        which_dev = handle_intr();
    } else if (cause == UserEnvCall) {
        if ((killed = iskilled(p)) != 0)
            exit(killed);

        // sepc points to the ecall instruction,
        // but we want to return to the next instruction.
        trapframe->epc += 4;

        // an interrupt will change sepc, scause, and sstatus,
        // so enable only now that we're done with those registers.
        intr_on();
        syscall();
        intr_off();
    } else if (cause == LoadPageFault || cause == StorePageFault || cause == InstructionPageFault) {
        handle_pgfault();
    } else {
        unknown_trap();
    }

    // are we still alive?
    if ((killed = iskilled(p)) != 0)
        exit(killed);

    // if it's a timer intr, call yield to give up CPU.
    if (which_dev == 1)
        yield();

    // prepare for return to user mode
    assert(!intr_get());
    usertrapret();
}

//
// return to user space
//
void usertrapret() {
    if (intr_get())
        panic("usertrapret entered with intr on");

    struct trapframe *trapframe = curr_proc()->trapframe;

    // set up trapframe values that uservec will need when
    // the process next traps into the kernel.
    trapframe->kernel_satp   = r_satp();                                 // kernel page table
    trapframe->kernel_sp     = curr_proc()->kstack + KERNEL_STACK_SIZE;  // process's kernel stack
    trapframe->kernel_trap   = (uint64)usertrap;                         // user's trap handler
    trapframe->kernel_hartid = r_tp();                                   // cpuid()

    // set S Exception Program Counter to the saved user pc.
    w_sepc(trapframe->epc);

    // set S Previous Privilege mode to User.
    uint64 x = r_sstatus();
    x &= ~SSTATUS_SPP;  // clear SPP to 0 for user mode
    x |= SSTATUS_SPIE;  // enable interrupts in user mode
    w_sstatus(x);

    // tell trampoline.S the user page table to switch to.
    uint64 satp  = MAKE_SATP(KVA_TO_PA(curr_proc()->mm->pgt));
    uint64 stvec = (TRAMPOLINE + (uservec - trampoline)) & ~0x3;

    // jump to userret in trampoline.S at the top of memory, which
    // switches to the user page table, restores user registers,
    // and switches to user mode with sret.
    uint64 fn = TRAMPOLINE + (userret - trampoline);
    tracef("return to user @%p, fn %p", trapframe->epc);
    ((void (*)(uint64, uint64, uint64))fn)(TRAPFRAME, satp, stvec);
}