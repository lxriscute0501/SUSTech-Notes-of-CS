#include "kalloc.h"

#include "defs.h"

struct linklist {
    struct linklist *next;
};

struct {
    struct linklist *freelist;
} kmem;

int kalloc_inited = 0;

extern uint64 __kva kpage_allocator_base;
extern uint64 __kva kpage_allocator_size;
static spinlock_t kpagelock;
int64 freepages_count;

void kpgmgrinit() {
    spinlock_init(&kpagelock, "pageallocator");

    uint64 kpage_allocator_end = kpage_allocator_base + kpage_allocator_size;

    infof("page allocator init: base: %p, stop: %p", kpage_allocator_base, kpage_allocator_end);

    assert(PGALIGNED(kpage_allocator_base));
    assert(PGALIGNED(kpage_allocator_end));

    for (uint64 p = kpage_allocator_end - PGSIZE; p >= kpage_allocator_base; p -= PGSIZE) {
        freepages_count++;
        kfreepage((void *)(p));
    }
    kalloc_inited = 1;
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfreepage(void *__pa pa) {
    struct linklist *l;

    uint64 __pa kvaddr = (uint64)pa;
    if (!PGALIGNED((uint64)pa) || !(kpage_allocator_base <= kvaddr && kvaddr < kpage_allocator_base + kpage_allocator_size))
        panic("invalid page %p", pa);
    memset((void *)kvaddr, 0xdd, PGSIZE);

    if (kalloc_inited)
        debugf("free: %p", pa);

    acquire(&kpagelock);
    l             = (struct linklist *)kvaddr;
    l->next       = kmem.freelist;
    kmem.freelist = l;
    freepages_count++;
    release(&kpagelock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *__pa kallocpage() {
    uint64 ra = r_ra();  // who calls me?

    acquire(&kpagelock);
    struct linklist *l;
    l = kmem.freelist;
    if (l) {
        kmem.freelist = l->next;
        freepages_count--;
    }
    release(&kpagelock);
    if (l)
        memset((char *)l, 0xaf, PGSIZE);
    return (void *)((uint64)l);
}

// Object Allocator
// static uint64 allocator_mapped_va = KERNEL_ALLOCATOR_BASE;

void allocator_init(struct allocator *alloc, char *name, uint64 object_size, uint64 count) {
    // Under NOMMU mode, we require the sizeof([header, object]) must be smaller than a page.
    //  because we cannot guarantee the [haeder,object] is in the same page.
    assert(object_size < PGSIZE - sizeof(struct linklist));

    // The allocator leaves spaces for a `struct linklist` before every object:
    //  [PGALIGNED][linklist, object][linklist, object]...[linklist, object]..[PGALIGNED]
    //             ^__pool_base, first object             ^_ the last obj               ^__pool_end

    memset(alloc, 0, sizeof(*alloc));
    // record basic properties of the allocator
    alloc->name = name;
    spinlock_init(&alloc->lock, "allocator");
    alloc->object_size         = object_size;
    alloc->object_size_aligned = ROUNDUP_2N(object_size + sizeof(struct linklist), 8);
    alloc->max_count           = count;

    uint64 addr = (uint64)kallocpage();
    assert(addr);
    memset((void *)addr, 0, PGSIZE);

    uint64 seg_start = addr, idx_start = 0;
    infof("allocator %s inited [NOMMU]. start: %p", name, seg_start);

    for (uint64 i = 0; i < count; i++) {
        if (((addr + alloc->object_size_aligned) >> PGSHIFT) != ((addr) >> PGSHIFT)) {
            // if next object will cross the page boundary, allocate a new page.
            uint64 next_page = (uint64)kallocpage();
            assert(next_page);
            memset((void *)next_page, 0, PGSIZE);

            // if we get the adjacent page, use the space between them.
            //  we can allocate next object just at `addr`.

            if ((next_page >> PGSHIFT) != (addr >> PGSHIFT) + 1) {
                // otherwise, we meet the non-continuous page, report it and set addr = next_page.
                infof(" - segment: [%d -> %d]: [%p -> %p)", idx_start, i, seg_start, addr);

                addr = next_page;

                seg_start = addr;
                idx_start = i;
            }

            // Note: if we have mmu to do address translate, we just simply let the VA to be continuous.
            //        we don't care whether the mapped PA is continuous or not.
        }

        struct linklist *l = (struct linklist *)addr;
        l->next            = alloc->freelist;
        alloc->freelist    = l;

        addr += alloc->object_size_aligned;
    }
    infof(" - segment: [%d -> %d]: [%p -> %p)", idx_start, count, seg_start, addr);

    alloc->available_count = alloc->max_count;
    alloc->allocated_count = 0;
}

void *kalloc(struct allocator *alloc) {
    assert(alloc);
    acquire(&alloc->lock);

    if (alloc->available_count == 0)
        panic("unavailable");

    alloc->available_count--;

    void *ret;

    struct linklist *l = alloc->freelist;
    if (l) {
        alloc->freelist = l->next;
        ret             = (void *)((uint64)l + sizeof(*l));

        alloc->allocated_count++;

        memset(l, 0xff, sizeof(*l));
        memset(ret, 0xfe, alloc->object_size);
    } else {
        panic("should be guarded by available_count");
    }
    release(&alloc->lock);

    tracef("kalloc(%s) returns %p", alloc->name, ret);

    return ret;
}

void kfree(struct allocator *alloc, void *obj) {
    if (obj == NULL)
        return;

    assert(alloc);
    // assert(alloc->pool_base <= (uint64)obj && (uint64)obj < alloc->pool_end);

    memset(obj, 0xfa, alloc->object_size);

    acquire(&alloc->lock);

    // put the object back to the freelist.
    struct linklist *l = (struct linklist *)((uint64)obj - sizeof(*l));
    l->next            = alloc->freelist;
    alloc->freelist    = l;

    alloc->allocated_count--;
    alloc->available_count++;
    assert(alloc->allocated_count + alloc->available_count == alloc->max_count);

    release(&alloc->lock);
}