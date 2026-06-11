#ifndef QUEUE_H
#define QUEUE_H

#include "lock.h"

#define QUEUE_SIZE (1024)

struct queue {
    spinlock_t lock;
    void *data[QUEUE_SIZE];
    int front;
    int tail;
    int empty;
};

void init_queue(struct queue *);
void push_queue(struct queue *, void *);
void *pop_queue(struct queue *);

#endif  // QUEUE_H
