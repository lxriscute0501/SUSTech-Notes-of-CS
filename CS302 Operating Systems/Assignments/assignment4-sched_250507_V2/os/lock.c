#include "lock.h"

#include "defs.h"

void spinlock_init(spinlock_t *lk, char *name)
{
	memset(lk, 0, sizeof(*lk));
	lk->name = name;
	lk->locked = 0;
	lk->cpu = 0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
void acquire(spinlock_t *lk)
{
	uint64 ra = r_ra();
	push_off(); // disable interrupts to avoid deadlock.
	if (holding(lk))
		panic("already acquired by %p, now %p", lk->where, ra);

	// On RISC-V, sync_lock_test_and_set turns into an atomic swap:
	//   a5 = 1
	//   s1 = &lk->locked
	//   amoswap.w.aq a5, a5, (s1)
	while (__sync_lock_test_and_set(&lk->locked, 1) != 0)
		;

	// Tell the C compiler and the processor to not move loads or stores
	// past this point, to ensure that the critical section's memory
	// references happen strictly after the lock is acquired.
	// On RISC-V, this emits a fence instruction.
	__sync_synchronize();

	// Record info about lock acquisition for holding() and debugging.
	lk->cpu = mycpu();
	lk->where = (void *)ra;
}

// Release the lock.
void release(spinlock_t *lk)
{
	if (!holding(lk))
		panic("release");

	lk->cpu = 0;
	lk->where = 0;

	// Tell the C compiler and the CPU to not move loads or stores
	// past this point, to ensure that all the stores in the critical
	// section are visible to other CPUs before the lock is released,
	// and that loads in the critical section occur strictly before
	// the lock is released.
	// On RISC-V, this emits a fence instruction.
	__sync_synchronize();

	// Release the lock, equivalent to lk->locked = 0.
	// This code doesn't use a C assignment, since the C standard
	// implies that an assignment might be implemented with
	// multiple store instructions.
	// On RISC-V, sync_lock_release turns into an atomic swap:
	//   s1 = &lk->locked
	//   amoswap.w zero, zero, (s1)
	__sync_lock_release(&lk->locked);

	pop_off();
}

// Check whether this cpu is holding the lock.
// Interrupts must be off.
int holding(spinlock_t *lk)
{
	int r;
	r = (lk->locked && lk->cpu == mycpu());
	return r;
}

// push_off/pop_off are like intr_off()/intr_on() except that they are matched:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

void push_off(void)
{
	uint64 ra = r_ra();

	int old = intr_get();
	intr_off();

	if (mycpu()->noff == 0) {
		// warnf("intr on saved: %p", ra);
		mycpu()->interrupt_on = old;
	}
	mycpu()->noff += 1;
}

void pop_off(void)
{
	uint64 ra = r_ra();

	struct cpu *c = mycpu();
	if (intr_get())
		panic("pop_off - interruptible");
	if (c->noff < 1)
		panic("pop_off - unpair");
	c->noff -= 1;
	if (c->noff == 0 && c->interrupt_on) {
		if (c->inkernel_trap)
			panic("pop_off->intr_on happens in kernel trap");
		intr_on();
	}
}

// void initsleeplock(struct sleeplock *lk, char *name)
// {
// 	spinlock_init(&lk->lk, "sleep lock");
// 	lk->name = name;
// 	lk->locked = 0;
// 	lk->pid = 0;
// }

// void acquiresleep(struct sleeplock *lk)
// {
// 	acquire(&lk->lk);
// 	while (lk->locked) {
// 		sleep(lk, &lk->lk);
// 	}
// 	lk->locked = 1;
// 	lk->pid = myproc()->pid;
// 	release(&lk->lk);
// }

// void releasesleep(struct sleeplock *lk)
// {
// 	acquire(&lk->lk);
// 	lk->locked = 0;
// 	lk->pid = 0;
// 	wakeup(lk);
// 	release(&lk->lk);
// }

// int holdingsleep(struct sleeplock *lk)
// {
// 	int r;

// 	acquire(&lk->lk);
// 	r = lk->locked && (lk->pid == myproc()->pid);
// 	release(&lk->lk);
// 	return r;
// }
