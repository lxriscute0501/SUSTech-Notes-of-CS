#include "defs.h"
#include "ktest.h"

extern int64 freepages_count;
extern allocator_t kstrbuf;

uint64 ktest_syscall(uint64 args[6]) {
    uint64 which = args[0];
    switch (which) {
        case KTEST_PRINT_USERPGT:
            vm_print(curr_proc()->mm->pgt);
            break;
        case KTEST_PRINT_KERNPGT:
            vm_print(kernel_pagetable);
            break;
        case KTEST_GET_NRFREEPGS:
            return freepages_count;
        case KTEST_GET_NRSTRBUF:
            return kstrbuf.available_count;
    }
    return 0;
}