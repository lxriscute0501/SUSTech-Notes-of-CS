#ifndef KALLOC_H
#define KALLOC_H

#include "vm.h"

void kpgmgrinit();
void kfreepage(void *pa);
void *__pa kallocpage();

// Object Allocator:

typedef struct allocator {
    char * name;
    spinlock_t lock;

    struct linklist *freelist;

    uint64 object_size;
    uint64 object_size_aligned;

    uint64 allocated_count;
    uint64 available_count;
    uint64 max_count;
} allocator_t;

void allocator_init(struct allocator *alloc, char *name, uint64 object_size, uint64 count);
void *kalloc(struct allocator *alloc);
void kfree(struct allocator *alloc, void *obj);

#endif // KALLOC_H