#include "plic.h"

#include "defs.h"
#include "trap.h"
#include "console.h"
//
// the riscv Platform Level Interrupt Controller (PLIC).
//
// see docs: https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc

void plicinit(void)
{
	// set desired IRQ priorities non-zero (otherwise disabled).
	// Interrupt source: UART0 - 10

	*(uint32 *)(KERNEL_PLIC_BASE + UART0_IRQ * 4) = 1;

	//   *(uint32*)(KERNEL_PLIC_BASE + VIRTIO0_IRQ*4) = 1;
}

void plicinithart(void)
{
	int hart = mycpu()->mhart_id;

	// base + 0x002000 + 0x80 + 0x100*hart
	// Assumption: Each hart has two context, we use the last one referring to the S-mode context.
	//	hart 0: context 1
	//	hart 1: context 3

	// set enable bits for this hart's S-mode for the uart.
	*(uint32 *)PLIC_SENABLE(hart) = (1 << UART0_IRQ);

	// set this hart's S-mode priority threshold to 0.
	*(uint32 *)PLIC_SPRIORITY(hart) = 0;

	w_sie(r_sie() | SIE_SEIE);	// enable External Interrupt
}

// ask the PLIC what interrupt we should serve.
int plic_claim(void)
{
	int hart = mycpu()->mhart_id;
	int irq = *(uint32 *)PLIC_SCLAIM(hart);
	return irq;
}

// tell the PLIC we've served this IRQ.
void plic_complete(int irq)
{
	int hart = mycpu()->mhart_id;
	*(uint32 *)PLIC_SCLAIM(hart) = irq;
}
