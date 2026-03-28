#ifndef PLIC_H
#define PLIC_H

#include "memlayout.h"
#include "types.h"

#define PLIC_PRIORITY        (KERNEL_PLIC_BASE + 0x0)
#define PLIC_PENDING         (KERNEL_PLIC_BASE + 0x1000)
#define PLIC_SENABLE(hart)   (KERNEL_PLIC_BASE + 0x2080 + (hart) * 0x100)
#define PLIC_SPRIORITY(hart) (KERNEL_PLIC_BASE + 0x201000 + (hart) * 0x2000)
#define PLIC_SCLAIM(hart)    (KERNEL_PLIC_BASE + 0x201004 + (hart) * 0x2000)

void plicinit(void);
void plicinithart(void);
int plic_claim(void);
void plic_complete(int);

#endif  // PLIC_H