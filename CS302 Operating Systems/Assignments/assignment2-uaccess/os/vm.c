#include "vm.h"

#include "defs.h"
#include "kalloc.h"

static allocator_t mm_allocator;
static allocator_t vma_allocator;

void uvm_init() {
    allocator_init(&mm_allocator, "mm", sizeof(struct mm), 16384);
    allocator_init(&vma_allocator, "vma", sizeof(struct vma), 16384);
}

// Return the address of the PTE in page table pagetable
// that corresponds to virtual address va.  If alloc!=0,
// create any required page-table pages.
//
// The risc-v Sv39 scheme has three levels of page-table
// pages. A page-table page contains 512 64-bit PTEs.
// A 64-bit virtual address is split into five fields:
//   39..63 -- must be zero.
//   30..38 -- 9 bits of level-2 index.
//   21..29 -- 9 bits of level-1 index.
//   12..20 -- 9 bits of level-0 index.
//    0..11 -- 12 bits of byte offset within the page.
pte_t *walk(struct mm *mm, uint64 va, int alloc) {
    assert(holding(&mm->lock));

    pagetable_t pagetable = mm->pgt;

    if (!IS_USER_VA(va))
        return NULL;

    for (int level = 2; level > 0; level--) {
        pte_t *pte = &pagetable[PX(level, va)];
        if (*pte & PTE_V) {
            pagetable = (pagetable_t)PA_TO_KVA(PTE2PA(*pte));
        } else {
            if (!alloc)
                return 0;
            void *pa = kallocpage();
            if (!pa)
                return 0;
            pagetable = (pagetable_t)PA_TO_KVA(pa);
            memset(pagetable, 0, PGSIZE);
            *pte = PA2PTE(KVA_TO_PA(pagetable)) | PTE_V;
        }
    }
    return &pagetable[PX(0, va)];
}

// Look up a virtual address, return the physical address,
// or 0 if not mapped.
// Can only be used to look up user pages.
uint64 __pa walkaddr(struct mm *mm, uint64 va) {
    if (!IS_USER_VA(va))
        return 0;

    assert_str(PGALIGNED(va), "unaligned va %p", va);
    assert(holding(&mm->lock));

    pte_t *pte;
    uint64 pa;

    pte = walk(mm, va, 0);
    if (pte == NULL)
        return 0;
    if ((*pte & PTE_V) == 0)
        return 0;
    if ((*pte & PTE_U) == 0) {
        warnf("walkaddr returns kernel pte: %p, %p", va, *pte);
        return 0;
    }
    pa = PTE2PA(*pte);
    return pa;
}

// Look up a virtual address, return the physical address,
uint64 useraddr(struct mm *mm, uint64 va) {
    uint64 page = walkaddr(mm, PGROUNDDOWN(va));
    if (page == 0)
        return 0;
    return page | (va & 0xFFFULL);
}

struct mm *mm_create() {
    struct mm *mm = kalloc(&mm_allocator);
    memset(mm, 0, sizeof(*mm));
    spinlock_init(&mm->lock, "mm");
    mm->vma    = NULL;
    mm->refcnt = 1;

    void *pa = kallocpage();
    if (!pa) {
        warnf("kallocpage failed");
        goto free_mm;
    }
    mm->pgt = (pagetable_t)PA_TO_KVA(pa);
    memset(mm->pgt, 0, PGSIZE);

    acquire(&mm->lock);
    return mm;

free_mm:
    kfree(&mm_allocator, mm);
    return NULL;
}

struct vma *mm_create_vma(struct mm *mm) {
    assert(holding(&mm->lock));

    struct vma *vma = kalloc(&vma_allocator);
    memset(vma, 0, sizeof(*vma));
    vma->owner = mm;
    return vma;
}

void mm_copy_kpgt(struct mm *mm) {
    assert(holding(&mm->lock));
    for (uint64 i = 0x100; i < 512; i++) {
        pte_t *kpte = (pte_t *)&kernel_pagetable[i];
        pte_t *upte = (pte_t *)&mm->pgt[i];
        *upte = *kpte;
    }
    sfence_vma();
}

static void freevma(struct vma *vma, int free_phy_page) {
    assert(holding(&vma->owner->lock));
    assert(PGALIGNED(vma->vm_start) && PGALIGNED(vma->vm_end));

    struct mm *mm = vma->owner;
    for (uint64 va = vma->vm_start; va < vma->vm_end; va += PGSIZE) {
        pte_t *pte = walk(mm, va, false);
        if (pte && (*pte & PTE_V)) {
            if (free_phy_page)
                kfreepage((void *)PTE2PA(*pte));
            *pte = 0;
        } else {
            debugf("free unmapped address %p", va);
        }
    }
    sfence_vma();
}

void mm_free_pages(struct mm *mm) {
    assert(holding(&mm->lock));

    struct vma *next, *vma = mm->vma;
    while (vma) {
        freevma(vma, true);
        next = vma->next;
        kfree(&vma_allocator, vma);
        vma = next;
    }
    mm->vma = NULL;
}

static void freepgt(pagetable_t pgt, uint64 va, int level) {
    for (int i = 0; i < 512; i++) {
        if ((pgt[i] & PTE_V) && (pgt[i] & PTE_RWX) == 0) {
            uint64 iva = va | (((uint64)i) << (12 + 9 * (2 - level)));
            if ((iva >> 38) & 1) {
                iva |= (~((1ull << 39) - 1));
            }
            if (!IS_KERN_VA(iva)) {
                // do not recursively free kernel pagetable.
                freepgt((pagetable_t)PA_TO_KVA(PTE2PA(pgt[i])), iva, level + 1);
            }
            pgt[i] = 0;
        }
    }
    kfreepage((void *)KVA_TO_PA(pgt));
}

void mm_free(struct mm *mm) {
    assert(holding(&mm->lock));
    assert(mm->refcnt > 0);

    mm_free_pages(mm);
    freepgt(mm->pgt, 0, 0);

    int oldref = mm->refcnt--;
    release(&mm->lock);

    if (oldref == 1) {
        kfree(&mm_allocator, mm);
    }
}

static int vma_check_overlap(struct mm *mm, uint64 start, uint64 end, struct vma *exclude) {
    assert(holding(&mm->lock));

    if (start == end)
        return 0;

    struct vma *vma = mm->vma;
    while (vma) {
        if (vma != exclude) {
            if ((start < vma->vm_end && start >= vma->vm_start) || (end > vma->vm_start && end <= vma->vm_end)) {
                return -1;
            }
        }
        vma = vma->next;
    }
    return 0;
}

/**
 * @brief Map virtual address defined in @vma.
 * Addresses must be aligned to PGSIZE.
 * Physical pages are allocated automatically.
 * Caller should then use walkaddr to resolve the mapped PA, and do initialization.
 *
 * @param vma
 * @return int
 */
int mm_mappages(struct vma *vma) {
    if (!IS_USER_VA(vma->vm_start) || !IS_USER_VA(vma->vm_end))
        panic("user mappages beyond USER_TOP, va: [%p, %p)", vma->vm_start, vma->vm_end);

    assert(PGALIGNED(vma->vm_start));
    assert(PGALIGNED(vma->vm_end));
    assert((vma->pte_flags & PTE_R) || (vma->pte_flags & PTE_W) || (vma->pte_flags & PTE_X));

    assert(holding(&vma->owner->lock));

    if (vma_check_overlap(vma->owner, vma->vm_start, vma->vm_end, vma)) {
        errorf("overlap: [%p, %p)", vma->vm_start, vma->vm_end);
        return -EINVAL;
    }

    tracef("mappages: [%p, %p)", vma->vm_start, vma->vm_end);

    struct mm *mm = vma->owner;
    uint64 va;
    void *pa;
    pte_t *pte;

    for (va = vma->vm_start; va < vma->vm_end; va += PGSIZE) {
        if ((pte = walk(mm, va, 1)) == 0) {
            errorf("pte invalid, va = %p", va);
            return -EINVAL;
        }
        if (*pte & PTE_V) {
            errorf("remap %p", va);
            return -EINVAL;
        }
        pa = kallocpage();
        if (!pa) {
            errorf("kallocpage");
            return -ENOMEM;
        }
        // memset((void *)PA_TO_KVA(pa), 0, PGSIZE);
        *pte = PA2PTE(pa) | vma->pte_flags | PTE_V;
    }
    sfence_vma();

    vma->next = mm->vma;
    mm->vma   = vma;

    return 0;
}

int mm_remap(struct vma *vma, uint64 start, uint64 end, uint64 pte_flags) {
    assert(PGALIGNED(start));
    assert(PGALIGNED(end));
    assert((pte_flags & PTE_R) || (pte_flags & PTE_W) || (pte_flags & PTE_X));
    debugf("remap: [%p, %p), flags = %p", start, end, pte_flags);

    pte_t *pte;
    struct mm *mm = vma->owner;
    assert(holding(&mm->lock));

    if (vma_check_overlap(mm, start, end, vma)) {
        errorf("overlap: [%p, %p)", start, end);
        return -EINVAL;
    }

    const uint64 iterstart = MIN(start, vma->vm_start);
    const uint64 iterend   = MAX(end, vma->vm_end);

    // first, consider all cases requiring new physical page.
    for (uint64 va = iterstart; va < iterend; va += PGSIZE) {
        if (va < start || va >= end) {
            // mapping to be removed.
            // however, we do not handle them now.
        } else {
            // mapping to be preseved or created.
            pte = walk(mm, va, 1);
            if (!pte) {
                errorf("remap: walk failed, va = %p", va);
                goto err;
            }
            if (*pte & PTE_V) {
                // mapping exists, update flags.
                uint64 pte_woflags = *pte & ~PTE_RWX;
                *pte               = pte_woflags | pte_flags;
            } else {
                // mapping does not exist, create it.
                void *pa = kallocpage();
                if (!pa) {
                    errorf("kallocpage, va = %p", va);
                    goto err;
                }
                *pte = PA2PTE(pa) | pte_flags | PTE_V;
            }
        }
    }

    // then, we are free from trying to allocate new physical pages.
    for (uint64 va = iterstart; va < iterend; va += PGSIZE) {
        if (va < start || va >= end) {
            // this mapping should be removed
            pte = walk(mm, va, 0);
            if (pte && (*pte & PTE_V)) {
                kfreepage((void *)PTE2PA(*pte));
                *pte = 0;
            } else {
                errorf("remap: mapping should exist, va = %p", va);
                return -EINVAL;
            }
        }
    }
    sfence_vma();

    vma->vm_start  = start;
    vma->vm_end    = end;
    vma->pte_flags = pte_flags;
    return 0;
err:
    // restore every mapping back
    for (uint64 va = iterstart; va < iterend; va += PGSIZE) {
        if (va < vma->vm_start || va >= vma->vm_end) {
            // this mapping should be removed
            pte = walk(mm, va, 0);
            if (pte && (*pte & PTE_V)) {
                kfreepage((void *)PTE2PA(*pte));
                *pte = 0;
            }
        } else {
            // mapping to be preseved.
            pte = walk(mm, va, 0);
            if (pte && (*pte & PTE_V)) {
                uint64 pte_woflags = *pte & ~PTE_RWX;
                *pte               = pte_woflags | vma->pte_flags;
            } else {
                panic_never_reach();
            }
        }
    }
    return -ENOMEM;
}

int mm_mappageat(struct mm *mm, uint64 va, uint64 __pa pa, uint64 flags) {
    assert(holding(&mm->lock));

    if (!IS_USER_VA(va))
        panic("invalid user VA");

    if (vma_check_overlap(mm, va, va + PGSIZE, NULL)) {
        errorf("overlap: [%p, %p)", va, va + PGSIZE);
        return -EINVAL;
    }

    tracef("mappagesat: %p -> %p", va, pa);

    pte_t *pte;

    if ((pte = walk(mm, va, 1)) == 0) {
        errorf("pte invalid, va = %p", va);
        return -EINVAL;
    }
    if (*pte & PTE_V) {
        errorf("remap %p", va);
        vm_print(mm->pgt);
        return -EINVAL;
    }
    *pte = PA2PTE(pa) | flags | PTE_V;
    sfence_vma();

    return 0;
}

// Used in fork.
// Copy the pagetable page and all the user pages.
// Return 0 on success, negative on error.
int mm_copy(struct mm *old, struct mm *new) {
    assert(holding(&old->lock));
    assert(holding(&new->lock));
    struct vma *vma = old->vma;

    while (vma) {
        tracef("fork: mapping [%p, %p)", vma->vm_start, vma->vm_end);
        struct vma *new_vma = mm_create_vma(new);
        new_vma->vm_start   = vma->vm_start;
        new_vma->vm_end     = vma->vm_end;
        new_vma->pte_flags  = vma->pte_flags;
        if (mm_mappages(new_vma)) {
            warnf("mm_mappages failed");
            // when failed, new_vma is not inserted into mm->vma list.
            //  free it by hand.
            freevma(new_vma, true);
            goto err;
        }
        for (uint64 va = vma->vm_start; va < vma->vm_end; va += PGSIZE) {
            void *__kva pa_old = (void *)PA_TO_KVA(walkaddr(old, va));
            void *__kva pa_new = (void *)PA_TO_KVA(walkaddr(new, va));
            memmove(pa_new, pa_old, PGSIZE);
        }
        vma = vma->next;
    }

    return 0;
err:
    mm_free_pages(new);
    return -ENOMEM;
}

struct vma* mm_find_vma(struct mm* mm, uint64 va) {
    assert(holding(&mm->lock));

    struct vma* vma = mm->vma;
    while (vma) {
        if (va == vma->vm_start) {
            return vma;
        }
        vma = vma->next;
    }
    return NULL;
}
