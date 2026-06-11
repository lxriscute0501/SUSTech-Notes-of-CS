#include "syscall.h"

#include "console.h"
#include "defs.h"
#include "ktest/ktest.h"
#include "loader.h"
#include "timer.h"
#include "trap.h"

int64 sys_fork() {
    return fork();
}

int64 sys_exec(uint64 __user path, uint64 __user argv) {
    int ret;
    char *kpath = kalloc(&kstrbuf);
    char *arg[MAXARG];
    memset(kpath, 0, KSTRING_MAX);
    memset(arg, 0, sizeof(arg));

    struct proc *p = curr_proc();

    acquire(&p->lock);
    acquire(&p->mm->lock);
    release(&p->lock);

    if ((ret = copystr_from_user(p->mm, kpath, path, KSTRING_MAX)) < 0) {
        goto free;
    }
    for (int i = 0; i < MAXARG; i++) {
        uint64 useraddr;
        if ((ret = copy_from_user(p->mm, (char *)&useraddr, argv + i * sizeof(uint64), sizeof(uint64))) < 0) {
            goto free;
        }
        if (useraddr == 0) {
            arg[i] = 0;
            break;
        }
        arg[i] = kalloc(&kstrbuf);
        assert(arg[i] != NULL);
        if ((ret = copystr_from_user(p->mm, arg[i], useraddr, KSTRING_MAX)) < 0) {
            goto free;
        }
    }
    release(&p->mm->lock);

    debugf("sys_exec %s\n", kpath);

    ret = exec(kpath, arg);

    kfree(&kstrbuf, kpath);
    for (int i = 0; arg[i]; i++) {
        kfree(&kstrbuf, arg[i]);
    }
    return ret;

free:
    release(&p->mm->lock);
    kfree(&kstrbuf, kpath);
    for (int i = 0; arg[i]; i++) {
        kfree(&kstrbuf, arg[i]);
    }
    return ret;
}

int64 sys_exit(int code) {
    exit(code);
    panic_never_reach();
}

int64 sys_wait(int pid, uint64 __user va) {
    struct proc *p = curr_proc();
    int *code      = NULL;

    acquire(&p->lock);
    acquire(&p->mm->lock);
    release(&p->lock);

    if (va != 0) {
        uint64 pa = useraddr(p->mm, va);
        code      = (int *)PA_TO_KVA(pa);
    }

    release(&p->mm->lock);

    return wait(pid, code);
}

int64 sys_getpid() {
    struct proc *cur = curr_proc();
    int pid;

    acquire(&cur->lock);
    pid = cur->pid;
    release(&cur->lock);

    return pid;
}

int64 sys_getppid() {
    struct proc *cur = curr_proc();
    int ppid;

    acquire(&cur->lock);
    ppid = cur->parent == NULL ? 0 : cur->parent->pid;
    release(&cur->lock);

    return ppid;
}

int64 sys_kill(int pid) {
    return kill(pid);
}

int64 sys_sleep(int64 n) {
    struct proc *p = curr_proc();

    acquire(&tickslock);
    uint64 ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (iskilled(p)) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

int64 sys_yield() {
    yield();
    return 0;
}

int64 sys_sbrk(int64 n) {
    int64 ret;
    struct proc *p = curr_proc();

    acquire(&p->lock);
    acquire(&p->mm->lock);

    struct vma *vma_brk = p->vma_brk;
    int64 old_brk       = p->brk;
    int64 new_brk       = (int64)p->brk + n;

    if (new_brk < vma_brk->vm_start) {
        warnf("userprog requested to shrink brk, but underflow.");
        ret = -EINVAL;
    } else {
        int64 roundup = PGROUNDUP(new_brk);
        if (roundup == vma_brk->vm_end) {
            ret = 0;
        } else {
            ret = mm_remap(vma_brk, vma_brk->vm_start, roundup, vma_brk->pte_flags);
        }
        if (ret == 0) {
            p->brk = new_brk;
        }
    }

    release(&p->mm->lock);
    release(&p->lock);

    if (ret == 0) {
        return old_brk;
    }
    return ret;
}

int64 sys_mmap() {
    panic("unimplemented");
}

int64 sys_read(int fd, uint64 __user va, uint64 len) {
    return user_console_read(va, len);
}

int64 sys_write(int fd, uint64 __user va, uint len) {
    return user_console_write(va, len);
}

void syscall() {
    struct trapframe *trapframe = curr_proc()->trapframe;
    int id                      = trapframe->a7;
    uint64 ret;
    uint64 args[6] = {trapframe->a0, trapframe->a1, trapframe->a2, trapframe->a3, trapframe->a4, trapframe->a5};
    tracef("syscall %d args = [%x, %x, %x, %x, %x, %x]", id, args[0], args[1], args[2], args[3], args[4], args[5]);
    switch (id) {
        case SYS_fork:
            ret = sys_fork();
            break;
        case SYS_exec:
            ret = sys_exec(args[0], args[1]);
            break;
        case SYS_exit:
            sys_exit(args[0]);
            panic_never_reach();
        case SYS_wait:
            ret = sys_wait(args[0], args[1]);
            break;
        case SYS_getpid:
            ret = sys_getpid();
            break;
        case SYS_getppid:
            ret = sys_getppid();
            break;
        case SYS_kill:
            ret = sys_kill(args[0]);
            break;
        case SYS_sleep:
            ret = sys_sleep(args[0]);
            break;
        case SYS_yield:
            ret = sys_yield();
            break;
        case SYS_sbrk:
            ret = sys_sbrk(args[0]);
            break;
        case SYS_mmap:
            ret = sys_mmap();
            break;
        case SYS_read:
            ret = sys_read(args[0], args[1], args[2]);
            break;
        case SYS_write:
            ret = sys_write(args[0], args[1], args[2]);
            break;
        case SYS_ktest:
            ret = ktest_syscall(args);
            break;
        default:
            ret = -1;
            errorf("unknown syscall %d", id);
    }
    trapframe->a0 = ret;
    tracef("syscall ret %d", ret);
}
