// init: The initial user-level program

#include "../lib/user.h"

char *argv[] = {"sh", NULL};

int main(void) {
    int pid, wpid;

    for (;;) {
        printf("init: starting sh\n");
        pid = fork();
        if (pid < 0) {
            printf("init: fork failed\n");
            exit(1);
        }
        if (pid == 0) {
            exec("sh", argv);
            printf("init: exec sh failed\n");
            exit(1);
        }

        for (;;) {
            // this call to wait() returns if the shell exits,
            // or if a parentless process exits.
            wpid = wait(-1, (int *)0);
            if (wpid == pid) {
                // the shell exited; restart it.
                printf("init: sh exited, restarting...\n");
                break;
            } else if (wpid < 0) {
                printf("init: wait returned an error\n");
                exit(1);
            } else {
                // it was a parentless process; do nothing.
                printf("init: wait a parentless process %d\n", wpid);
            }
        }
    }
}
