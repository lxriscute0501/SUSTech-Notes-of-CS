#include "defs.h"
#include "timer.h"

#define NTHREAD 15
#define CNT_PER_THR 10000
#define SLEEP_TIME 50

volatile uint64 count = 0;

void worker(uint64 id) {
    int pid = curr_proc()->pid;
    warnf("thread %d: starting", pid);
    for (int i = 0; i < CNT_PER_THR; i++) {
        count++;
        if (count % 1000 == 0) {
            if (!intr_get())
                panic("interrupt should be on when executing process");

            infof("thread %d: count %d, sleeping", pid, count);
            sleepms(SLEEP_TIME);
        }
    }
    warnf("thread %d: exiting", pid);
    exit(pid + 20);
}


void init(uint64) {
    infof("kthread: init starts!");
    int pids[NTHREAD];
    for (int i = 0; i < NTHREAD; i++) {
        pids[i]        = create_kthread(worker, i);
    }
    int retcode;
    for (int i = 0; i < NTHREAD; i++) {
        int pid = wait(pids[i], &retcode);
        infof("thread %d exited with code %d, expected %d", pid, retcode, pid + 20);
    }
    infof("all threads exited, count %d\n", count);
    infof("init ends!");
    exit(0);
}
