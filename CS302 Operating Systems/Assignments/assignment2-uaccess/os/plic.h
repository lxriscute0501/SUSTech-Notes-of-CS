#ifndef PLIC_H
#define PLIC_H

#include "memlayout.h"
#include "types.h"

#define PLIC_PRIORITY        (KERNEL_PLIC_BASE + 0x0)
#define PLIC_PENDING         (KERNEL_PLIC_BASE + 0x1000)
#define PLIC_SENABLE(ctx)    (KERNEL_PLIC_BASE + 0x2000 + (ctx) * 0x80)
#define PLIC_SPRIORITY(ctx)  (KERNEL_PLIC_BASE + 0x200000 + (ctx) * 0x1000)
#define PLIC_SCLAIM(ctx)     (KERNEL_PLIC_BASE + 0x200004 + (ctx) * 0x1000)

void plicinit(void);
void plicinithart(void);
int plic_claim(void);
void plic_complete(int);

#endif  // PLIC_H