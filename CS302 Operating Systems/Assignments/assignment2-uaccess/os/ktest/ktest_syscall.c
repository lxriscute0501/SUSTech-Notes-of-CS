#include "defs.h"
#include "ktest.h"

const char* secret = "This is a secret message!\n";

uint64 ktest_syscall(uint64 args[6]) {
    uint64 which = args[0];
    switch (which) {
        case KTEST_PRINT_USERPGT:
            vm_print(curr_proc()->mm->pgt);
            break;
        case KTEST_PRINT_KERNPGT:
            vm_print(kernel_pagetable);
            break;
        case 99:
            // Assignment 2: checkpoint 1
            {
                uint64 pgt, pte1, pte2, pte;
                pgt = PA_TO_KVA(SATP_TO_PGTABLE(r_satp()));

                // 0, 2, 2 -> 0x0000000000402000, V,RX,U
                pte1 = ((uint64*)pgt)[0];
                if (!(pte1 & PTE_V))
                    return 1;
                pte1 = PA_TO_KVA(PTE2PA(pte1));
                pte2 = ((uint64*)pte1)[2];
                if (!(pte2 & PTE_V))
                    return 2;
                pte2 = PA_TO_KVA(PTE2PA(pte2));
                pte  = ((uint64*)pte2)[2];
                if (!(pte & PTE_V) || !(pte & PTE_R) || !(pte & PTE_X) || !(pte & PTE_U))
                    return 3;

                /*
[1fe], pte[0xffffffc080d14ff0]: 0xffffffff80000000 -> 0x0000000080401000 --G----V
  [1], pte[0xffffffc080401008]: 0xffffffff80200000 -> 0x0000000080402000 --G----V
    [0], pte[0xffffffc080402000]: 0xffffffff80200000 -> 0x0000000080200000 -AG-X-RV
*/
                pte1 = ((uint64*)pgt)[0x1fe];
                if (!(pte1 & PTE_V))
                    return 4;
                pte1 = PA_TO_KVA(PTE2PA(pte1));
                pte2 = ((uint64*)pte1)[1];
                if (!(pte2 & PTE_V))
                    return 5;
                pte2 = PA_TO_KVA(PTE2PA(pte2));
                pte  = ((uint64*)pte2)[0];
                if (!(pte & PTE_V) || !(pte & PTE_R) || !(pte & PTE_X) || (pte & PTE_U))
                    return 6;
            }
            return 0;
        case 100:
            return (uint64)secret;
    }
    return 0;
}