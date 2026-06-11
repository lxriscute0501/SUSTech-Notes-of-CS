#include "defs.h"
#include "ktest.h"

extern int64 freepages_count;
extern allocator_t kstrbuf;

void assignment3_copytouser(uint64 useraddr, uint64 uservalue) {
    int32 v       = uservalue;
    struct mm* mm = curr_proc()->mm;
    acquire(&mm->lock);
    copy_to_user(mm, useraddr, (void*)&v, sizeof(v));
    release(&mm->lock);
}

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
        case KTEST_A3_COPY_TO_USER:
            assignment3_copytouser(args[1], args[2]);
            return 0;
    }
    return 0;
}