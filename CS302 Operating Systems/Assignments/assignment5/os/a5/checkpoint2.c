#include "defs.h"
#include "log.h"

// checkpoint 2: Semaphore has a counter
static semaphore_t sem;
static uint64 count;
static uint64 done;

#define ROUNDS 5000
#define NTHRS 8
#define SEM_VALUE 5

static void monitor(uint64 id) {
    assert(intr_get());
    intr_off();
    // for the monitor, we never allow it to be preempted.
    while (__atomic_load_n(&done, __ATOMIC_ACQUIRE) == 0) {
        uint64 cnt = __atomic_load_n(&count, __ATOMIC_ACQUIRE);
        assert_str(cnt <= SEM_VALUE, "count %d", cnt);
    }
    intr_on();
    exit(0);
}

static void worker(uint64 id) {
    for (int i = 0; i < ROUNDS; i++) {
        sem_wait(&sem);
        __atomic_add_fetch(&count, 1, __ATOMIC_ACQ_REL);
        yield();
        __atomic_sub_fetch(&count, 1, __ATOMIC_ACQ_REL);
        sem_post(&sem);
        
        if (i % (ROUNDS / 10) == 0) {
            infof("w %d round %d",id, i);
        }
    }
    exit(0);
}

void checkpoint2(void) {
    sem_init(&sem, SEM_VALUE);
    count = 0;
    done = 0;

    infof("-> checkpoint 2 starts");

    int mon = create_kthread(monitor, 0);

    int pids[NTHRS];

    for (int i = 0; i < NTHRS; i++) {
        // Create n thread to run the checkpoint 1 code
        pids[i] = create_kthread(worker, i);
    }
    for (int i = 0; i < NTHRS; i++) {
        wait(pids[i], NULL);
    }
    __atomic_store_n(&done, 1, __ATOMIC_RELEASE);
    wait(mon, NULL);

    infof("-> checkpoint 2 passed");
}
