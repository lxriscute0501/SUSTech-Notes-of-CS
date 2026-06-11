#include "../lib/user.h"

char hugebuf[4096 * (1000 - 19)];
// verybig should use exactly 1000 pages of memory.
// 19 pages are used by the stack, pagetable and so on.

int main() {
    sleep(10);
    exit(1);
    return 0;
}