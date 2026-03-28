#include "defs.h"
#include "kalloc.h"
#include "proc.h"
#include "queue.h"
#include "trap.h"

static struct queue task_queue;

// defined in proc.c
extern struct proc *pool[NPROC];

void sched_init() {
    init_queue(&task_queue);
}

static struct proc *fetch_task() {
    struct proc *proc = pop_queue(&task_queue);
    if (proc != NULL)
        debugf("fetch task (pid=%d) from task queue", proc->pid);
    return proc;
}

void add_task(struct proc *p) {
    assert(p->state == RUNNABLE);
    assert(holding(&p->lock));

    push_queue(&task_queue, p);
    debugf("add task (pid=%d) to task queue", p->pid);
}

static int all_dead() {
    push_off();
    int alive = 0;
    for (int i = 0; i < NPROC; i++) {
        struct proc *p = pool[i];
        acquire(&p->lock);
        if (p->state != UNUSED)
            alive = true;
        release(&p->lock);
        if (alive)
            break;
    }
    pop_off();
    return !alive;
}

// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void scheduler() {
    struct proc *p;
    struct cpu *c = mycpu();

    // We only get here once.
    // After each cpu boots, it calls scheduler().
    // If this scheduler finds any possible process to run, it will switch to it.
    // 	And the scheduler context is saved on "mycpu()->sched_context"

    for (;;) {
        // intr may be on here.

        p = fetch_task();
        if (p == NULL) {
            // if we cannot find a process in the task_queue
            //  maybe some processes are SLEEPING and some are RUNNABLE
            if (all_dead()) {
                panic("[cpu %d] scheduler dead.", c->cpuid);
            } else {
                // nothing to run; stop running on this core until an interrupt.
                intr_on();
                asm volatile("wfi");
                intr_off();
                continue;
            }
        }

        acquire(&p->lock);
        assert(p->state == RUNNABLE);
        logf(PURPLE, "sched", "switch to proc pid(%d)", p->pid);
        p->state = RUNNING;
        c->proc  = p;
        swtch(&c->sched_context, &p->context);

        // When we get back here, someone must have called swtch(..., &c->sched_context);
        assert(c->proc == p);
        assert(!intr_get());        // scheduler should never have intr_on()
        assert(holding(&p->lock));  // whoever switch to us must acquire p->lock
        c->proc = NULL;

        if (p->state == RUNNABLE) {
            add_task(p);
        }
        release(&p->lock);
    }
}

// Switch to scheduler.  Must hold only p->lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->noff, but that would
// break in the few places where a lock is held but
// there's no process.
void sched() {
    int interrupt_on;
    struct proc *p = curr_proc();

    if (!holding(&p->lock))
        panic("not holding p->lock");
    if (mycpu()->noff != 1)
        panic("holding another locks");
    if (p->state == RUNNING)
        panic("sched running process");
    if (mycpu()->inkernel_trap)
        panic("sched should never be called in kernel trap context.");
    assert(!intr_get());

    interrupt_on = mycpu()->interrupt_on;
    logf(PURPLE, "sched", "switch to scheduler pid(%d)", p->pid);
    swtch(&p->context, &mycpu()->sched_context);
    mycpu()->interrupt_on = interrupt_on;

    // if scheduler returns here: p->lock must be holding.
    if (!holding(&p->lock))
        panic("not holding p->lock after sched.swtch returns");
}

// Give up the CPU for one scheduling round.
void yield() {
    struct proc *p = curr_proc();
    debugf("yield: (%d)%p", p->pid, p);

    acquire(&p->lock);
    p->state = RUNNABLE;
    sched();
    release(&p->lock);
}
