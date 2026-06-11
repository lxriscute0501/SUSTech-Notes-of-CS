#include "../lib/user.h"
#include "../../os/ktest/ktest.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <which checkpoint>\n", argv[0]);
        return 1;
    }
    int which = atoi(argv[1]);
    if (which == 1) {
        int64 ret = ktest(99, NULL, 0);
        assert(ret == 0);
    } else if (which == 2) {
        write(1, "test write\n", 11);
    } else if (which == 3) {
        uint64 kaddr = ktest(100, NULL, 0);
        printf("-> kernel secret addr: %p\n", kaddr);
        printf("-> Your copy_from_user should not leak kernel data\n", kaddr);

        int ret = write(1, (void*)kaddr, 30);
        assert(ret < 0);
    } else if (which == 4) {
        int pid = fork();
        if (pid == 0) {
            write(1, (void*)0x80000000, 30);
            exit(0);
        } else {
            int code;
            int ret = wait(pid, &code);
            assert(ret == pid);
            assert(code == -9);
        }
    } else {
        printf("Invalid checkpoint: %d\n", which);
        return 1;
    }
    printf("-> checkpoint %d passed\n", which);
    return 0;
}