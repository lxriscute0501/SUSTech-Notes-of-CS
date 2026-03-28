#include "sbi.h"
#include "types.h"
#include "riscv.h"

const uint64 SBI_SET_TIMER = 0;
const uint64 SBI_CONSOLE_PUTCHAR = 1;
const uint64 SBI_CONSOLE_GETCHAR = 2;
const uint64 SBI_CLEAR_IPI = 3;
const uint64 SBI_SEND_IPI = 4;
const uint64 SBI_REMOTE_FENCE_I = 5;
const uint64 SBI_REMOTE_SFENCE_VMA = 6;
const uint64 SBI_REMOTE_SFENCE_VMA_ASID = 7;
const uint64 SBI_SHUTDOWN = 8;

const uint64 SBI_HSM = 0x48534D;

static int inline sbi_call_legacy(uint64 which, uint64 arg0, uint64 arg1, uint64 arg2)
{
	register uint64 a0 asm("a0") = arg0;
	register uint64 a1 asm("a1") = arg1;
	register uint64 a2 asm("a2") = arg2;
	register uint64 a7 asm("a7") = which;
	asm volatile("ecall" : "=r"(a0) : "r"(a0), "r"(a1), "r"(a2), "r"(a7) : "memory");
	return a0;
}

static struct sbiret inline sbi_call(int32 eid, int32 fid, uint64 arg0, uint64 arg1, uint64 arg2)
{
	register uint64 a0 asm("a0") = arg0;
	register uint64 a1 asm("a1") = arg1;
	register uint64 a2 asm("a2") = arg2;
	register uint64 a6 asm("a6") = fid;
	register uint64 a7 asm("a7") = eid;
	asm volatile("ecall" : "=r"(a0), "=r"(a1) : "r"(a0), "r"(a1), "r"(a2), "r"(a6), "r"(a7) : "memory");
	struct sbiret ret;
	ret.error = a0;
	ret.value = a1;
	return ret;
}
void sbi_putchar(int c)
{
	sbi_call_legacy(SBI_CONSOLE_PUTCHAR, c, 0, 0);
}

int sbi_hsm_hart_start(unsigned long hartid, unsigned long start_addr, unsigned long a1)
{
	struct sbiret ret = sbi_call(SBI_HSM, 0x0, hartid, start_addr, a1);
	return ret.error;
}

void shutdown()
{
	intr_off();
	while (1)
		asm volatile("nop" ::: "memory");
	sbi_call_legacy(SBI_SHUTDOWN, 0, 0, 0);
}

void set_timer(uint64 stime)
{
	sbi_call_legacy(SBI_SET_TIMER, stime, 0, 0);
}