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
        kfreepage((void *)KVA_TO_PA(p));
        freepages_count++;
    }
    kalloc_inited = 1;
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfreepage(void *__pa pa) {
    struct linklist *l;

    uint64 __kva kvaddr = PA_TO_KVA(pa);
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
    
    debugf("alloc: %p, by %p", KVA_TO_PA(l), ra);

    if (l != NULL) {
        memset((char *)l, 0xaf, PGSIZE);  // fill with junk
    } else {
        warnf("out of memory, called by %p", ra);
        return 0;
    }
    return (void *)KVA_TO_PA((uint64)l);
}

// Object Allocator
static uint64 allocator_mapped_va = KERNEL_ALLOCATOR_BASE;

void allocator_init(struct allocator *alloc, char *name, uint64 object_size, uint64 count) {
    // Under NOMMU mode, we require the sizeof([header, object]) is smaller than a page.
    // assert(object_size < PGSIZE - sizeof(struct linklist));

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

    assert(count <= PGSIZE * 8);

    // calculate how many pages do we need
    uint64 total_size = alloc->object_size_aligned * alloc->max_count;
    total_size        = PGROUNDUP(total_size);

    // calculate the pool base and end.
    alloc->pool_base = allocator_mapped_va;
    alloc->pool_end  = alloc->pool_base + total_size;

    infof("allocator %s inited base %p", name, alloc->pool_base);

    // add a significant gap between different types of objects.
    allocator_mapped_va += ROUNDUP_2N(total_size, KERNEL_ALLOCATOR_GAP);

    // allocate physical pages and kvmmap [pool_base, pool_end)
    for (uint64 va = alloc->pool_base; va < alloc->pool_end; va += PGSIZE) {
        void *__pa pg = kallocpage();
        if (pg == NULL)
            panic("kallocpage");
        memset((void *)PA_TO_KVA(pg), 0xf8, PGSIZE);
        kvmmap(kernel_pagetable, va, (uint64)pg, PGSIZE, PTE_A | PTE_D | PTE_R | PTE_W);
    }
    sfence_vma();

    // init the freelist:
    for (uint64 i = 0, addr = alloc->pool_base; i < alloc->max_count; i++) {
        assert(addr + alloc->object_size_aligned <= alloc->pool_end);
        struct linklist *l = (struct linklist *)addr;
        l->next            = alloc->freelist;
        alloc->freelist    = l;
        addr += alloc->object_size_aligned;
    }

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
    assert(alloc->pool_base <= (uint64)obj && (uint64)obj < alloc->pool_end);

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