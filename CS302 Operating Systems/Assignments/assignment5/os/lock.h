#ifndef __LOCK_H__
#define __LOCK_H__

#include "types.h"

// Mutual exclusion lock.
struct spinlock {
    uint64 locked;  // Is the lock held?, use AMO instructions to access this field.

    // For debugging:
    char *name;       // Name of lock.
    struct cpu *cpu;  // The cpu holding the lock.
    void *where;      // who calls acquire?
};

// Long-term locks for processes
struct sleeplock {
    uint locked;         // Is the lock held?
    struct spinlock lk;  // spinlock protecting this sleep lock

    // For debugging:
    char *name;  // Name of lock.
    int pid;     // Process holding lock
};

// Assignment 5: Semaphore
struct semaphore {
    int value;
    struct spinlock lk;
};

typedef struct spinlock spinlock_t;
typedef struct sleeplock sleeplock_t;
typedef struct semaphore semaphore_t;

void spinlock_init(struct spinlock *lk, char *name);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
int holding(struct spinlock *lk);
void push_off(void);
void pop_off(void);

// Sleeplock:
void initsleeplock(sleeplock_t *lk, char *name);
void acquiresleep(sleeplock_t *lk);
void releasesleep(sleeplock_t *lk);
int holdingsleep(sleeplock_t *lk);

// Assignment 5: Semaphore
void sem_init(semaphore_t *sem, int value);
void sem_wait(semaphore_t *sem);
void sem_post(semaphore_t *sem);

#endif  //  __LOCK_H__
