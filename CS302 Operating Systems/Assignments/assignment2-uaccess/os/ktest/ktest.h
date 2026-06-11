#ifndef __KTEST_H__
#define __KTEST_H__

#include "../types.h"
uint64 ktest_syscall(uint64 args[6]);

#define KTEST_PRINT_USERPGT 1
#define KTEST_PRINT_KERNPGT 2
#define KTEST_GET_NRFREEPGS 3
#define KTEST_GET_NRSTRBUF  4

#endif  // __KTEST_H__