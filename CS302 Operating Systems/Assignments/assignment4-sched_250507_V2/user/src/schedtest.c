#include "../../os/ktest/ktest.h"
#include "../lib/user.h"

void worker() {
    unsigned long long cnt = 0;
    for (int i = 0; i < 300; i++) {
        // printf("worker %d: %d\n", getpid(), i);

        for (long j = 0; j < 10000000; j++) {
            asm volatile("" : : : "memory");
            cnt++;
            // do nothing
        }

        // sleep(100);
    }
}

uint64 rtime() {
    // uint64 x;
    // asm volatile("csrr %0, time" : "=r"(x));
    return ktest(KTEST_GET_TICKS, 0, 0);
}

#define NTIMES 10
#define NPROCS 8

int main(void) {
    struct {
        int pid;
        int timeused;
    } results[NPROCS];

    // run the test 10 times
    for (int j = 0; j < 10; j++) {
        printf("schedtest: running %d-th test\n", j);

        // fork `NPROCS` times
        for (int i = 0; i < NPROCS; i++) {
            int pid = fork();
            if (pid < 0) {
                printf("init: fork failed\n");
                exit(1);
            }
            if (pid == 0) {
                // child process

                // set priority, i:[0-7]
                // May 7th Revision: make priority in the reverse order regarding the pid
                setpriority(7 - i);
                printf("worker %d: priority %d\n", getpid(), 7 - i);
                yield();

                uint64 start = rtime();
                printf("worker %d: starts at %l\n", getpid(), start);
                worker();
                uint64 end = rtime();
                printf("worker %d: exits  at %l\n", getpid(), end);

                // calculate the time used as the exit code
                int used = end - start;
                exit(used);
            } else {
                // parent
                results[i].pid      = pid;
                results[i].timeused = 0;
            }
        }

        // wait all children
        int ret, pid;
        while ((pid = wait(-1, &ret)) > 0) {
            for (int i = 0; i < NPROCS; i++) {
                if (results[i].pid == pid) {
                    results[i].timeused = ret;
                    break;
                }
            }
        }

        // check results: lower pid (higher `priority` value) should take more time
        for (int i = 0; i < NPROCS; i++) {
            printf("schedtest: worker pid %d: used %d time\n", results[i].pid, results[i].timeused);
        }
        // finally there should be a significant gap between the first and the last
        assert(results[0].timeused >= results[NPROCS - 1].timeused + 30);

        // calculate the slope via the least squares method:

        long avg_t = 0, avg_y = 0;
        for (int i = 0; i < NPROCS; i++) {
            avg_t += i * 10;
            avg_y += results[i].timeused;
        }
        avg_t /= NPROCS;
        avg_y /= NPROCS;

        long b = 0, denom = 0;
        for (int i = 0; i < NPROCS; i++) {
            b += (i * 10 - avg_t) * (results[i].timeused - avg_y);
            denom += (i * 10 - avg_t) * (i * 10 - avg_t);
        }
        int final = (b * 10000) / denom;
        printf("schedtest: final slope: %d\n", final);

        assert(final < 0 && -final >= 8000);
    }

    printf("\n\n===\nschedtest: 10-times tests passed\n===\n\n");

    return 0;
}