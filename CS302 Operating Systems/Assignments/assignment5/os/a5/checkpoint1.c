#include "defs.h"
#include "log.h"

// checkpoint 1: Semaphore as a mutex
static semaphore_t ckpt1;
static volatile uint64 count;

#define ROUNDS 10000
#define NTHRS 4

static void worker(uint64 id) {
    for (int i = 0; i < ROUNDS; i++) {
        sem_wait(&ckpt1);
        count++;
        sem_post(&ckpt1);
        
        if (i % (ROUNDS / 10) == 0) {
            infof("w %d round %d",id, i);
        }
    }
    exit(0);
}

void checkpoint1(void) {
    // Initialize the semaphore with value 1
    sem_init(&ckpt1, 1);
    count = 0;

    sem_wait(&ckpt1);
    // you must not simply use a spinlock to do so.
    assert(getcpu(r_tp())->noff == 0);
    // interrupt must be on.
    assert(intr_get());
    sem_post(&ckpt1);

    infof("-> checkpoint 1 starts");

    for (int i = 0; i < NTHRS; i++) {
        // Create a thread to run the checkpoint 1 code
        create_kthread(worker, i);
    }
    while (wait(-1, NULL) > 0);

    assert(count == NTHRS * ROUNDS);

    infof("-> checkpoint 1 passed");
}
