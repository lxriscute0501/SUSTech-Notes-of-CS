#include "proc.h"

#include "defs.h"
#include "kalloc.h"
#include "queue.h"
#include "trap.h"

struct proc *pool[NPROC];
struct proc *init_proc = NULL;
static allocator_t proc_allocator;

static spinlock_t pid_lock;
static spinlock_t wait_lock;

extern void sched_init();

// initialize the proc table at boot time.
void proc_init() {
    // we only init once.
    static int proc_inited = 0;
    assert(proc_inited == 0);
    proc_inited = 1;

    spinlock_init(&pid_lock, "pid");
    spinlock_init(&wait_lock, "wait");

    allocator_init(&proc_allocator, "proc", sizeof(struct proc), NPROC);
    struct proc *p;

    for (int i = 0; i < NPROC; i++) {
        p = kalloc(&proc_allocator);
        memset(p, 0, sizeof(*p));
        spinlock_init(&p->lock, "proc");
        p->index = i;
        p->state = UNUSED;

        p->kstack = (uint64)kallocpage();
        assert(p->kstack);

        pool[i] = p;
    }
    sched_init();
}

static int allocpid() {
    static int PID = 1;
    int retpid     = -1;

    acquire(&pid_lock);
    retpid = PID++;
    release(&pid_lock);

    return retpid;
}
static void first_sched_ret(void) {
    // s0: frame pointer, s1: fn, s2: uint64 arg
    void (*fn)(uint64);
    uint64 arg;
    asm volatile("mv %0, s1":"=r"(fn));
    asm volatile("mv %0, s2":"=r"(arg));
    
    release(&curr_proc()->lock);
    intr_on();
    fn(arg);
    panic("first_sched_ret should never return. You should use exit to terminate kthread");
}

int create_kthread(void (*fn)(uint64), uint64 arg) {
    struct proc *p = allocproc();
    if (!p)
        return -1;

    // initialize process state
    p->context.ra = (uint64)first_sched_ret;
    p->context.sp = p->kstack + PGSIZE;
    p->context.s1 = (uint64)fn;
    p->context.s2 = arg;
    p->state = RUNNABLE;
    p->parent = init_proc;

    int pid = p->pid;
    add_task(p);
    release(&p->lock);

    return pid;
}

// Look in the process table for an UNUSED proc.
// If found, initialize state required to run in the kernel.
// If there are no free procs, or a memory allocation fails, return 0.
struct proc *allocproc() {
    struct proc *p;
    for (int i = 0; i < NPROC; i++) {
        p = pool[i];
        acquire(&p->lock);
        if (p->state == UNUSED) {
            goto found;
        }
        release(&p->lock);
    }
    return 0;

found:
    // initialize a proc
    tracef("init proc %p", p);
    p->pid        = allocpid();
    p->state      = USED;
    p->killed     = 0;
    p->sleep_chan = NULL;
    p->parent     = NULL;
    p->exit_code  = 0;

    memset(&p->context, 0, sizeof(p->context));
    memset((void *)p->kstack, 0, PGSIZE);

    if (!init_proc)
        init_proc = p;

    assert(holding(&p->lock));

    return p;

    // Resources clean up.
}

static void freeproc(struct proc *p) {
    assert(holding(&p->lock));

    p->state      = UNUSED;
    p->pid        = -1;
    p->exit_code  = 0xdeadbeef;
    p->sleep_chan = NULL;
    p->killed     = 0;
    p->parent     = NULL;
}

void sleep(void *chan, spinlock_t *lk) {
    struct proc *p = curr_proc();

    // Must acquire p->lock in order to
    // change p->state and then call sched.
    // Once we hold p->lock, we can be
    // guaranteed that we won't miss any wakeup
    // (wakeup locks p->lock),
    // so it's okay to release lk.

    acquire(&p->lock);  // DOC: sleeplock1
    release(lk);

    // Go to sleep.
    p->sleep_chan = chan;
    p->state      = SLEEPING;

    sched();

    // p get waking up, Tidy up.
    p->sleep_chan = 0;

    // Reacquire original lock.
    release(&p->lock);
    acquire(lk);
}

void wakeup(void *chan) {
    for (int i = 0; i < NPROC; i++) {
        struct proc *p = pool[i];
        acquire(&p->lock);
        if (p->state == SLEEPING && p->sleep_chan == chan) {
            p->state = RUNNABLE;
            add_task(p);
        }
        release(&p->lock);
    }
}

int wait(int pid, int *code) {
    struct proc *child;
    int havekids;
    struct proc *p = curr_proc();

    acquire(&wait_lock);

    for (;;) {
        // Scan through table looking for exited children.
        havekids = 0;
        for (int i = 0; i < NPROC; i++) {
            child = pool[i];
            if (child == p)
                continue;

            acquire(&child->lock);
            if (child->parent == p) {
                havekids = 1;
                if (child->state == ZOMBIE && (pid <= 0 || child->pid == pid)) {
                    int cpid = child->pid;
                    // Found one.
                    if (code)
                        *code = child->exit_code;
                    freeproc(child);
                    release(&child->lock);
                    release(&wait_lock);
                    return cpid;
                }
            }
            release(&child->lock);
        }

        // No waiting if we don't have any children.
        if (!havekids || p->killed) {
            release(&wait_lock);
            return -1;
        }

        debugf("pid %d sleeps for wait", p->pid);
        // Wait for a child to exit.
        sleep(p, &wait_lock);  // DOC: wait-sleep
    }
}

// Exit the current process.
void exit(int code) {
    struct proc *p = curr_proc();

    if (p == init_proc) {
        panic("init process exited");
    }

    acquire(&wait_lock);

    int wakeinit = 0;

    // reparent:
    for (int i = 0; i < NPROC; i++) {
        struct proc *child = pool[i];
        if (child == p)
            continue;
        acquire(&child->lock);
        if (child->parent == p){
            child->parent = init_proc;
            wakeinit = 1;
            // if child has dead, wake up init to do clean up.
        }
        release(&child->lock);
    }
    if (wakeinit)
        wakeup(init_proc);

    // wakeup wait-ing parent.
    //  There is no race because locking against "wait_lock"
    wakeup(p->parent);

    acquire(&p->lock);

    p->exit_code = code;
    p->state     = ZOMBIE;

    release(&wait_lock);

    sched();
    panic_never_reach();
}
