#ifndef VM_H
#define VM_H

#include "lock.h"
#include "riscv.h"
#include "types.h"

#define __user
#define __pa
#define __kva

// These two macros are used to convert between kernel virtual address and physical address,
//  BUT ONLY FOR symbols defined in kernel image.
#define KIVA_TO_PA(x) (((uint64)(x)) - KERNEL_OFFSET)
#define PA_TO_KIVA(x) (((uint64)(x)) + KERNEL_OFFSET)

#define KVA_TO_PA(x) (((uint64)(x)) - KERNEL_DIRECT_MAPPING_BASE)
#define PA_TO_KVA(x) (((uint64)(x)) + KERNEL_DIRECT_MAPPING_BASE)

#define IS_USER_VA(x) (((uint64)(x)) <= MAXVA)
#define IS_KERN_VA(x) (((uint64)(x)) >= KERNEL_DIRECT_MAPPING_BASE)

extern uint64 __pa kernel_image_end_4k;
extern uint64 __pa kernel_image_end_2M;
extern pagetable_t kernel_pagetable;

struct kernelmap {
    uint32 valid;
    uint32 vpn2_index;
    pte_t pte;
};

struct mm;
struct vma {
    struct mm* owner;
    struct vma* next;
    uint64 vm_start;
    uint64 vm_end;
    uint64 pte_flags;
};
struct mm {
    spinlock_t lock;

    pagetable_t __kva pgt;
    struct vma* vma;
    int refcnt;
};

// kvm.c
void kvm_init();
void kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm);

// vm.c
void uvm_init();

pte_t* walk(struct mm* mm, uint64 va, int alloc);
uint64 __pa walkaddr(struct mm* mm, uint64 va);
uint64 useraddr(struct mm* mm, uint64 va);

struct mm* mm_create();
struct vma* mm_create_vma(struct mm* mm);
void mm_copy_kpgt(struct mm* mm);
void mm_free_pages(struct mm* mm);
void mm_free(struct mm* mm);
int mm_mappages(struct vma* vma);
int mm_remap(struct vma *vma, uint64 start, uint64 end, uint64 pte_flags);
int mm_mappageat(struct mm *mm, uint64 va, uint64 __pa pa, uint64 flags);
int mm_copy(struct mm* old, struct mm* new);
struct vma* mm_find_vma(struct mm* mm, uint64 va);

// uaccess.c
int copy_to_user(struct mm* mm, uint64 __user dstva, char* src, uint64 len);
int copy_from_user(struct mm* mm, char* dst, uint64 __user srcva, uint64 len);
int copystr_from_user(struct mm* mm, char* dst, uint64 __user srcva, uint64 max);

void vm_print(pagetable_t pagetable);

#endif  // VM_H
