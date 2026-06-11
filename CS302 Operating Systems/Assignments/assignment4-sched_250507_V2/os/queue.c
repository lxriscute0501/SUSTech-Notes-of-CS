#include "queue.h"

#include "defs.h"
#include "log.h"
#include "proc.h"

void init_queue(struct queue *q) {
    spinlock_init(&q->lock, "queue");
    q->front = q->tail = 0;
    q->empty           = 1;
}

void push_queue(struct queue *q, void *data) {
    acquire(&q->lock);
    if (!q->empty && q->front == q->tail) {
        panic("queue overflow");
    }
    q->empty         = 0;
    q->data[q->tail] = data;
    q->tail          = (q->tail + 1) % NPROC;
    release(&q->lock);
}

void *pop_queue(struct queue *q) {
    acquire(&q->lock);
    if (q->empty) {
        release(&q->lock);
        return NULL;
    }

    void *data = q->data[q->front];
    q->front   = (q->front + 1) % NPROC;
    if (q->front == q->tail)
        q->empty = 1;
    release(&q->lock);
    return data;
}
