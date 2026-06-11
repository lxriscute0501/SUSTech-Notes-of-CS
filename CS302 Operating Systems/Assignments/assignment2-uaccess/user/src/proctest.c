#include "../../os/ktest/ktest.h"
#include "../../os/riscv.h"
#include "../lib/user.h"

// regression test. test whether exec() leaks memory if one of the
// arguments is invalid. the test passes if the kernel doesn't panic.
void exec_badarg(char *s) {
    for (int i = 0; i < 50000; i++) {
        char *argv[2];
        argv[0] = (char *)0xffffffff;
        argv[1] = 0;
        int ret = exec("echo", argv);
        if (ret >= 0) {
            printf("exec_badarg: exec succeeded with bad arg\n");
            exit(1);
        }
    }

    exit(0);
}

// test if child is killed (status = -1)
void killstatus(char *s) {
    int xst;

    for (int i = 0; i < 100; i++) {
        int pid1 = fork();
        if (pid1 < 0) {
            printf("%s: fork failed\n", s);
            exit(1);
        }
        if (pid1 == 0) {
            while (1) {
                getpid();
            }
            exit(0);
        }
        sleep(1);
        kill(pid1);
        wait(-1, &xst);
        if (xst != -1) {
            printf("%s: status should be -1\n", s);
            exit(1);
        }
    }
    exit(0);
}

// try to find any races between exit and wait
void exitwait(char *s) {
    int i, pid;

    for (i = 0; i < 100; i++) {
        pid = fork();
        if (pid < 0) {
            printf("%s: fork failed\n", s);
            exit(1);
        }
        if (pid) {
            int xstate;
            if (wait(-1, &xstate) != pid) {
                printf("%s: wait wrong pid\n", s);
                exit(1);
            }
            if (i != xstate) {
                printf("%s: wait wrong exit status\n", s);
                exit(1);
            }
        } else {
            exit(i);
        }
    }
}

// try to find races in the reparenting
// code that handles a parent exiting
// when it still has live children.
void reparent(char *s) {
    int master_pid = getpid();
    for (int i = 0; i < 200; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("%s: fork failed\n", s);
            exit(1);
        }
        if (pid) {
            if (wait(-1, NULL) != pid) {
                printf("%s: wait wrong pid\n", s);
                exit(1);
            }
        } else {
            int pid2 = fork();
            if (pid2 < 0) {
                kill(master_pid);
                exit(1);
            }
            exit(0);
        }
    }
    exit(0);
}

// concurrent forks to try to expose locking bugs.
void forkfork(char *s) {
    enum { N = 2 };

    for (int i = 0; i < N; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("%s: fork failed", s);
            exit(1);
        }
        if (pid == 0) {
            for (int j = 0; j < 200; j++) {
                int pid1 = fork();
                if (pid1 < 0) {
                    exit(1);
                }
                if (pid1 == 0) {
                    exit(0);
                }
                wait(-1, 0);
            }
            exit(0);
        }
    }

    int xstatus;
    for (int i = 0; i < N; i++) {
        wait(-1, &xstatus);
        if (xstatus != 0) {
            printf("%s: fork in child failed", s);
            exit(1);
        }
    }
}

void sbrkbasic(char *s) {
    enum { TOOMUCH = 1024 * 1024 * 1024 };
    int i, pid, xstatus;
    char *c, *a, *b;

    // does sbrk() return the expected failure value?
    pid = fork();
    if (pid < 0) {
        printf("fork failed in sbrkbasic\n");
        exit(1);
    }
    if (pid == 0) {
        a = sbrk(TOOMUCH);
        if (a == (char *)0xffffffffffffffffL) {
            // it's OK if this fails.
            exit(0);
        }

        for (b = a; b < a + TOOMUCH; b += 4096) {
            *b = 99;
        }

        // we should not get here! either sbrk(TOOMUCH)
        // should have failed, or (with lazy allocation)
        // a pagefault should have killed this process.
        exit(1);
    }

    wait(-1, &xstatus);
    if (xstatus == 1) {
        printf("%s: too much memory allocated!\n", s);
        exit(1);
    }

    // can one sbrk() less than a page?
    a = sbrk(0);
    for (i = 0; i < 5000; i++) {
        b = sbrk(1);
        if (b != a) {
            printf("%s: sbrk test failed %d %p %p\n", s, i, a, b);
            exit(1);
        }
        *b = 1;
        a  = b + 1;
    }
    pid = fork();
    if (pid < 0) {
        printf("%s: sbrk test fork failed\n", s);
        exit(1);
    }
    c = sbrk(1);
    c = sbrk(1);
    if (c != a + 1) {
        printf("%s: sbrk test failed post-fork\n", s);
        exit(1);
    }
    if (pid == 0)
        exit(0);
    wait(-1, &xstatus);
    exit(xstatus);
}

void sbrkmuch(char *s) {
    enum { BIG = 100 * 1024 * 1024 };
    char *c, *oldbrk, *a, *lastaddr, *p;
    uint64 amt;

    oldbrk = sbrk(0);

    // can one grow address space to something big?
    a   = sbrk(0);
    amt = BIG - (uint64)a;
    p   = sbrk(amt);
    if (p != a) {
        printf("%s: sbrk test failed to grow big address space; enough phys mem?\n", s);
        exit(1);
    }

    // touch each page to make sure it exists.
    char *eee = sbrk(0);
    for (char *pp = a; pp < eee; pp += 4096) *pp = 1;

    lastaddr  = (char *)(BIG - 1);
    *lastaddr = 99;

    // can one de-allocate?
    a = sbrk(0);
    c = sbrk(-PGSIZE);
    if (c == (char *)0xffffffffffffffffL) {
        printf("%s: sbrk could not deallocate\n", s);
        exit(1);
    }
    c = sbrk(0);
    if (c != a - PGSIZE) {
        printf("%s: sbrk deallocation produced wrong address, a %p c %p\n", s, a, c);
        exit(1);
    }

    // can one re-allocate that page?
    a = sbrk(0);
    c = sbrk(PGSIZE);
    if (c != a || sbrk(0) != a + PGSIZE) {
        printf("%s: sbrk re-allocation failed, a %p c %p\n", s, a, c);
        exit(1);
    }
    if (*lastaddr == 99) {
        // should be zero
        printf("%s: sbrk de-allocation didn't really deallocate\n", s);
        exit(1);
    }

    a = sbrk(0);
    c = sbrk(-((char *)sbrk(0) - oldbrk));
    if (c != a) {
        printf("%s: sbrk downsize failed, a %p c %p\n", s, a, c);
        exit(1);
    }
}

// does uninitialized data start out zero?
char uninit[10000];
void bsstest(char *s) {
    int i;

    for (i = 0; i < sizeof(uninit); i++) {
        if (uninit[i] != '\0') {
            printf("%s: bss test failed\n", s);
            exit(1);
        }
    }
}

// check that writes to a few forbidden addresses
// cause a fault, e.g. process's text and TRAMPOLINE.
void nowrite(char *s) {
    int pid;
    int xstatus;
    uint64 addrs[] = {0, 0x80000000LL, 0x3fffffe000, 0x3ffffff000, 0x4000000000, 0xffffffffffffffff};

    for (int ai = 0; ai < sizeof(addrs) / sizeof(addrs[0]); ai++) {
        pid = fork();
        if (pid == 0) {
            volatile int *addr = (int *)addrs[ai];
            *addr              = 10;
            printf("%s: write to %p did not fail!\n", s, addr);
            exit(0);
        } else if (pid < 0) {
            printf("%s: fork failed\n", s);
            exit(1);
        }
        wait(-1, &xstatus);
        if (xstatus == 0) {
            // kernel did not kill child!
            exit(1);
        }
    }
    exit(0);
}

struct test {
    void (*f)(char *);
    char *s;
} proctests[] = {
    {exec_badarg, "exec_badarg"},
    {killstatus,  "killstatus" },
    {exitwait,    "exitwait"   },
    {reparent,    "reparent"   },
    {forkfork,    "forkfork"   },
    {sbrkbasic,   "sbrkbasic"  },
    {sbrkmuch,    "sbrkmuch"   },
    {bsstest,     "bsstest"    },
    {nowrite,     "nowrite"    },
    {NULL,        NULL         },
};

int run(void f(char *), char *s) {
    int pid;
    int xstatus;

    printf("test %s: ", s);
    if ((pid = fork()) < 0) {
        printf("runtest: fork error\n");
        exit(1);
    }
    if (pid == 0) {
        f(s);
        exit(0);
    } else {
        wait(-1, &xstatus);
        if (xstatus != 0)
            printf("FAILED\n");
        else
            printf("OK\n");
        return xstatus == 0;
    }
}

int runtests(struct test *tests, char *whichone, int continuous) {
    for (struct test *t = tests; t->s != 0; t++) {
        if ((whichone == NULL) || strcmp(t->s, whichone) == 0) {
            if (!run(t->f, t->s)) {
                if (continuous != 2) {
                    printf("SOME TESTS FAILED\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}

int drivetests(int quick, int continuous, char *whichone) {
    do {
        printf("usertests starting\n");
        int freepg  = ktest(KTEST_GET_NRFREEPGS, 0, 0);
        int freebuf = ktest(KTEST_GET_NRSTRBUF, 0, 0);
        if (runtests(proctests, whichone, continuous)) {
            if (continuous != 2) {
                return 1;
            }
        }
        int freepg1  = ktest(KTEST_GET_NRFREEPGS, 0, 0);
        int freebuf1 = ktest(KTEST_GET_NRSTRBUF, 0, 0);
        if (freepg1 < freepg || freebuf < freebuf1) {
            printf("FAILED -- lost some free pages %d (out of %d), kstrbuf: %d (out of %d)\n", freepg1, freepg, freebuf1, freebuf);
            if (continuous != 2) {
                return 1;
            }
        }
    } while (continuous);
    return 0;
}

int main() {
    printf("=== TESTSUITE ===\nproctest\n\n");
    drivetests(0, 0, NULL);
    return 0;
}