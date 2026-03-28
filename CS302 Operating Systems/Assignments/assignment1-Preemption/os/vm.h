#ifndef VM_H
#define VM_H

#include "lock.h"
#include "riscv.h"
#include "types.h"

#define __user
#define __pa
#define __kva

// no mmu mode, make compile error if these macro are used.

#define KIVA_TO_PA(x) ((void)sizeof(ERROR_NOMMU_MODE), 0)
#define PA_TO_KIVA(x) ((void)sizeof(ERROR_NOMMU_MODE), 0)

#define KVA_TO_PA(x) ((void)sizeof(ERROR_NOMMU_MODE), 0)
#define PA_TO_KVA(x) ((void)sizeof(ERROR_NOMMU_MODE), 0)

#define IS_USER_VA(x) ((void)sizeof(ERROR_NOMMU_MODE), 0)

#endif  // VM_H
