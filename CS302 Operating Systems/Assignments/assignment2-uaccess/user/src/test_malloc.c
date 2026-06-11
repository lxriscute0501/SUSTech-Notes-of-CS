#include "../lib/user.h"

int main(int argc, char *argv[]) {
    printf("argc = %d\n", argc);
    if (argc != 5) {
        printf("Usage: %s <size> <count> <pre fork> <fork count>\n", argv[0]);
        return 1;
    }
    int size     = atoi(argv[1]);
    int count    = atoi(argv[2]);
    int prefork  = atoi(argv[3]);
    int postfork = atoi(argv[4]);
    printf("size = %d, count = %d, prefork = %d, postfork = %d\n", size, count, prefork, postfork);
    for (int i = 0; i < prefork; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("fork failed\n");
            return 1;
        }
        if (pid == 0) {
            printf("[prefork] child %d\n", getpid());
            break;
        }
    }
    int pid = getpid();
    printf("[%d] malloc start\n", pid);
    for (int i = 0; i < count; i++) {
        void *p = malloc(size);
        if (p == NULL) {
            printf("malloc failed\n");
            return 1;
        }
        printf("[%d] malloc %p, %d/%d\n", pid, p, i, count);
    }
    printf("[%d] malloc done\n", pid);
    for (int i = 0; i < postfork; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("fork failed\n");
            return 1;
        }
        if (pid == 0) {
            printf("child %d\n", getpid());
            return 0;
        }
    }

    return 0;
}
