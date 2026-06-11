#include "../lib/user.h"

char hugebuf[4096 * (100 - 18)];
// big should use exactly 100 pages of memory.
// 19 pages are used by the stack, pagetable and so on.

int main() {
    sleep(10);
    exit(1);
    return 0;
}