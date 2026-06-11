#ifndef SBI_H
#define SBI_H

#include "types.h"

struct sbiret {
	long error;
	long value;
};

void sbi_putchar(int);
void shutdown();
void set_timer(uint64 stime);
int sbi_hsm_hart_start(unsigned long hartid, unsigned long start_addr, unsigned long a1);
uint64 sbi_get_mvendorid(void);
uint64 sbi_get_mimpid(void);

#endif // SBI_H
