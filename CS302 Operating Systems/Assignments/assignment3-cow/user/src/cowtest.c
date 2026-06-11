#include "../../os/ktest/ktest.h"
#include "../../os/riscv.h"
#include "../lib/user.h"

#define getfreemem() (ktest(KTEST_GET_NRFREEPGS, 0, 0))

/**
 * @brief Test whether we really implement CoW.
 * fork, exec "verybig", which will consume 1000 available pages in kernel.
 * Then, expand the heap size until the heap consumes more pages than the free pages.
 * Finally, try to fork again.
 * CoW fork will succeed, because they are sharing the heap, but the normal fork will fail.
 */
int test1(char *name) {
    char *exec_verybig[] = {"verybig", NULL};

    const int initial_nfree = getfreemem();
    printf("setup: initial free pages: %d\n", initial_nfree);
    int nfree = initial_nfree;
    int pid;
    do {
        pid = fork();
        assert_str(pid >= 0, "fork should not fail here");
        if (pid == 0) {
            // child
            exec(exec_verybig[0], exec_verybig);
            exit(102);
        }
        sleep(1);
        nfree = getfreemem();
        printf("setup: remaining free pages: %d, initial free pages: %d\n", nfree, initial_nfree);
    } while (nfree >= 1500);
    // Note: we deliberately do not wait for the child process, even if they turn into ZOMBIE.
    //  Pages allocated by them will not be freed until we wait them.

    nfree = getfreemem();
    printf("setup: fork-ends: remaining free pages: %d, initial free pages: %d\n", nfree, initial_nfree);

    // Then, expand the heap size, until our heap size is larger than the nf_freepages
    void *origbrk  = sbrk(0);
    void *brk      = origbrk;
    int heap_pages = 0;
    do {
        brk = sbrk(4096);
        assert(brk > 0);
        brk        = sbrk(0);
        nfree      = getfreemem();
        heap_pages = ((uint64)brk - (uint64)origbrk) / 4096;
        printf("setup: heap expanded: %d pages, free pages: %d\n", heap_pages, nfree);
    } while (heap_pages < nfree + 20);

    printf("%s: the number of free pages is less than the heap size.\n", name);
    printf("%s: let's try the CoW fork.\n", name);

    // Print user's pagetable, uncomment if you need:
    // ktest(KTEST_PRINT_USERPGT, 0, 0);

    int failed = 0;
    int status;

    // Finally, test whether we can fork.
    pid = fork();
    if (pid < 0) {
        failed = 1;
        goto clean;
    }
    if (pid == 0) {
        for (int i = 0; i < 10; i++) sleep(1);
        printf("-> %s - I'm the child process\n", name);
        exit(104);
    } else if (pid > 0) {
        printf("-> %s - I'm the parent process\n", name);
        assert(wait(pid, &status) == pid);
        assert_str(status == 104, "child process should exit with code 104");
    }
    printf("-> %s - single fork passed\n", name);

    // fork 10 times, are your implementation really do CoW?
    int pids[10];
    for (int i = 0; i < 10; i++) {
        pids[i] = -1;
    }
    for (int i = 0; i < 10; i++) {
        pid = fork();
        if (pid < 0) {
            failed = 1;
            for (int i = 0; i < 10; i++) {
                if (pids[i] > 0) {
                    kill(pids[i]);
                }
            }
            goto clean;
        }
        if (pid == 0) {
            // child
            while (1) sleep(1);
            exit(99);  // never exits.
        }
        pids[i] = pid;
        printf("-> %s - multiple fork: %d, pid: %d, free pages: %d\n", name, i, pid, getfreemem());
    }
    // kill & wait, check return status
    for (int i = 0; i < 10; i++) {
        kill(pids[i]);
        assert(wait(pids[i], &status) == pids[i]);
        assert_str(status == -1, "child process should exit with code -1 (be killed)");
        printf("-> %s - multiple fork: %d, pid: %d, exited\n", name, i, pids[i]);
    }

clean:
    while (wait(-1, NULL) > 0);
    if (failed) {
        printf("-> %s - failed\n", name);
    } else {
        printf("-> %s - passed\n", name);
    }
    return failed;
}

int test2(char *name) {
    const int size   = 50 * PGSIZE;
    const int stride = PGSIZE / sizeof(int);

    int *const pheap = (int *)sbrk(size);
    assert((uint64)pheap > 0);
    memset(pheap, 0, size);

    int *const pheapend = pheap + size / sizeof(int);

    int parentpid = getpid();
    // parent: fill them with my pid:
    for (int *p = pheap; p < pheapend; p += stride) {
        *p = parentpid;
    }
    printf(" -> %s - allocate heap: %d pages, filling with mypid\n", name, size / PGSIZE);

    // Case 1: fork, then let child read them:
    printf(" -> %s - CoW fork, child read\n", name);
    int status = -1;
    int pid    = fork();
    assert_str(pid >= 0, "fork should not fail here");

    if (pid == 0) {
        // child
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == parentpid);
        }
        exit(0);
    } else {
        assert(wait(pid, &status) == pid);
        assert_str(status == 0, "child should exit with code 0");
    }

    // parent: fill them with my pid:
    for (int *p = pheap; p < pheapend; p += stride) {
        *p = parentpid;
    }

    // Case 2: fork, then let child to write them:
    printf(" -> %s - CoW fork, child write\n", name);
    pid = fork();
    assert_str(pid >= 0, "fork should not fail here");

    if (pid == 0) {
        // child
        int childpid = getpid();
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == parentpid);
            *p = childpid;
            sleep(1);  // use sleep to make interleave.
        }
        exit(0);
    } else {
        assert(wait(pid, &status) == pid);
        assert_str(status == 0, "child should exit with code 0");
        // after child exited, try to read from parent's pages.
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == parentpid);
        }
    }

    // parent: fill them with my pid:
    for (int *p = pheap; p < pheapend; p += stride) {
        *p = parentpid;
    }

    // Case 3: fork, then let child and parent to write them, but parent runs in a reverse order.
    printf(" -> %s - CoW fork, child and parent write\n", name);
    pid = fork();
    assert_str(pid >= 0, "fork should not fail here");

    if (pid == 0) {
        // child
        int childpid = getpid();
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == parentpid);
            *p = childpid;
            sleep(1);  // use sleep to make interleave.
        }
        for (int *p = pheapend - stride; p >= pheap; p -= stride) {
            assert(*p == childpid);
        }
        exit(0);
    } else {
        for (int *p = pheapend - stride; p >= pheap; p -= stride) {
            assert(*p == parentpid);
            *p = 0xdeadbeef;
            sleep(1);
        }
        assert(wait(pid, &status) == pid);
        assert_str(status == 0, "child should exit with code 0");
        // after child exited, try to read from parent's pages.
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == 0xdeadbeef);
        }
    }

    return 0;
}

int test3(char *name) {
    const int size   = 50 * PGSIZE;
    const int stride = PGSIZE / sizeof(int);

    // setup: make system has less than 200 pages.

    {
        char *exec_big[]        = {"big", NULL};
        const int initial_nfree = getfreemem();
        printf("setup: initial free pages: %d\n", initial_nfree);
        int nfree = initial_nfree;
        int pid;
        do {
            pid = fork();
            assert_str(pid >= 0, "fork should not fail here");
            if (pid == 0) {
                // child
                exec(exec_big[0], exec_big);
                exit(102);
            }
            sleep(1);
            nfree = getfreemem();
            printf("setup: remaining free pages: %d, initial free pages: %d\n", nfree, initial_nfree);
        } while (nfree >= 250);
    }

    int *const pheap = (int *)sbrk(size);
    assert((uint64)pheap > 0);
    memset(pheap, 0, size);

    int *const pheapend = pheap + size / sizeof(int);

    int parentpid = getpid();
    // parent: fill them with my pid:
    for (int *p = pheap; p < pheapend; p += stride) {
        *p = parentpid;
    }
    printf(" -> %s - allocate heap: %d pages, filling with mypid\n", name, size / PGSIZE);

    // Case 1: fork, then let child and parent to write them, but use copy_to_user in kernel.
    printf(" -> %s - CoW fork, copy_to_user should also do CoW\n", name);
    int pid, status;
    pid = fork();
    assert_str(pid >= 0, "fork should not fail here");

    if (pid == 0) {
        // child
        int childpid = getpid();
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == parentpid);
            // *p = childpid;
            ktest(KTEST_A3_COPY_TO_USER, p, childpid);
            sleep(1);  // use sleep to make interleave.
        }
        for (int *p = pheapend - stride; p >= pheap; p -= stride) {
            assert(*p == childpid);
        }
        exit(0);
    } else {
        for (int *p = pheapend - stride; p >= pheap; p -= stride) {
            assert(*p == parentpid);
            // *p = 0xdeadbeef;
            ktest(KTEST_A3_COPY_TO_USER, p, 0xdeadbeef);
            sleep(1);
        }
        assert(wait(pid, &status) == pid);
        assert_str(status == 0, "child should exit with code 0");
        // after child exited, try to read from parent's pages.
        for (int *p = pheap; p < pheapend; p += stride) {
            assert(*p == 0xdeadbeef);
        }
    }

    // Case 2: fork, then check whether Read-Only becomes writable.
    printf(" -> %s - CoW fork, read-only remains read-only\n", name);
    const char *pstr = "hello world";  // this should points to a read-only page.
    pid              = fork();
    assert_str(pid >= 0, "fork should not fail here");

    if (pid == 0) {
        // child
        char *p = (char *)pstr;
        // although we cast it to a char*, it still points to a read-only page.
        *p = 'X';
        // this should cause a page fault.
        exit(0);
    } else {
        assert(wait(pid, &status) == pid);
        assert_str(status != 0, "child should be killed by kernel");
    }

    while (wait(-1, NULL) > 0);
    return 0;
}

void runtest(int checkleak, int (*func)(char *), char *funcname, char *name) {
    int nfree = getfreemem();

    int pid, status;
    pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        exit(func(name));
    } else {
        assert(wait(pid, &status) == pid);
        if (status != 0) {
            printf("test failed.\n");
            return;
        }
        if (!checkleak)
            return;
        int nowfree = getfreemem();
        printf("-> %s - check whether you leak any page\n", name);
        if (nowfree != nfree) {
            printf("-> %s - failed:\n  - after %s exits: free pages: %d, initial: %d\n", name, funcname, nowfree, nfree);
        } else {
            printf("-> %s - passed\n", name);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Assignment 3 - Copy-on-Write - usertest: \n");
        printf("Usage: %s <which checkpoint>\n", argv[0]);
        return 1;
    }
    int which = atoi(argv[1]);
    if (which == 1) {
        runtest(0, test1, "test1", "checkpoint1");
    } else if (which == 2) {
        runtest(1, test1, "test1", "checkpoint2");
    } else if (which == 3) {
        runtest(1, test2, "test2", "checkpoint3");
    } else if (which == 4) {
        runtest(1, test3, "test3", "checkpoint4");
    } else {
        printf("Invalid checkpoint: %d\n", which);
        return 1;
    }
    return 0;
}