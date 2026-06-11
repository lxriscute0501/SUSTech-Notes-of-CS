#include "defs.h"
#include "vm.h"

pagetable_t kernel_pagetable;
static uint64 __kva init_page_allocator;
static uint64 __kva init_page_allocator_base;

uint64 __kva kpage_allocator_base;
uint64 __kva kpage_allocator_size;

static pagetable_t kvmmake();
void kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm);

// Now we have a base Direct Mapping Area:
// 	starts: KERNEL_DIRECT_MAPPING_BASE + kernel_image_end_2M
//	size: 2MiB.

static uint64 __kva allocsetuppage() {
    assert(init_page_allocator >= init_page_allocator_base);
    assert(init_page_allocator < init_page_allocator_base + PGSIZE_2M);
    assert(PGALIGNED(init_page_allocator));

    uint64 __kva pg = init_page_allocator;
    init_page_allocator += PGSIZE;
    debugf("%p", pg);
    return pg;
}

static uint64 __kva allockernelpage() {
    extern int kalloc_inited;
    if (kalloc_inited) {
        void *__pa pa = kallocpage();
        if (pa == 0)
            panic("out of memory");
        return PA_TO_KVA(pa);
    }
    return allocsetuppage();
}

// Initialize the one kernel_pagetable
// Switch h/w page table register to the kernel's page table,
// and enable paging.
void kvm_init() {
    init_page_allocator      = KERNEL_DIRECT_MAPPING_BASE + kernel_image_end_2M;
    init_page_allocator_base = init_page_allocator;
    infof("boot-stage page allocator: base %p, end %p", init_page_allocator, init_page_allocator + PGSIZE_2M);

    kernel_pagetable = kvmmake();

    uint64 satp = MAKE_SATP(KVA_TO_PA(kernel_pagetable));
    sfence_vma();
    asm volatile("csrw satp, %0" : : "r"(satp));
    sfence_vma();
    infof("enable pageing at %p", r_satp());

    infof("boot-stage page allocator ends up: base %p, used: %p", init_page_allocator_base, init_page_allocator);
    init_page_allocator = 0xdead;

    // infof("enable sstatus.SUM == 0", r_sstatus());
    // w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

// Make a kernel mapping page table for the kernel.
static pagetable_t kvmmake() {
    pagetable_t kpgtbl;
    kpgtbl = (pagetable_t)allocsetuppage();
    memset(kpgtbl, 0, PGSIZE);

    // Step.1 : Kernel Image
    // map kernel text executable and read-only.
    // 0xffff_ffff_8020_0000 -> 0x8020_0000
    kvmmap(kpgtbl, (uint64)s_text, KIVA_TO_PA(s_text), (uint64)e_text - (uint64)s_text, PTE_A | PTE_R | PTE_X);

    // map kernel ro_data: s_rodata to e_rodata
    kvmmap(kpgtbl, (uint64)s_rodata, KIVA_TO_PA(s_rodata), (uint64)e_rodata - (uint64)s_rodata, PTE_A | PTE_R);

    // map kernel .s_data to .e_bss,
    uint64 kimage_data_size = KIVA_TO_PA(e_bss) - KIVA_TO_PA(s_data);
    kvmmap(kpgtbl, (uint64)s_data, KIVA_TO_PA(s_data), kimage_data_size, PTE_A | PTE_D | PTE_R | PTE_W);

    // Step.2 : Kernel Trampoline
    // map trampoline
    kvmmap(kpgtbl, (uint64)TRAMPOLINE, KIVA_TO_PA(trampoline), PGSIZE, PTE_A | PTE_R | PTE_X);

    // Step.3 : Kernel Device MMIO :
    kvmmap(kpgtbl, KERNEL_PLIC_BASE, PLIC_PHYS, KERNEL_PLIC_SIZE, PTE_A | PTE_D | PTE_R | PTE_W);
    kvmmap(kpgtbl, KERNEL_UART0_BASE, UART0_PHYS, KERNEL_UART0_SIZE, PTE_A | PTE_D | PTE_R | PTE_W);

    // Step.4 : Kernel Scheduler stack:
    uint64 sched_stack = KERNEL_STACK_SCHED;
    for (int i = 0; i < NCPU; i++) {
        struct cpu *c = getcpu(i);
        // allocate #KERNEL_STACK_SIZE / PGSIZE pages
        for (uint64 va = sched_stack; va < sched_stack + KERNEL_STACK_SIZE; va += PGSIZE) {
            uint64 __pa newpg = KVA_TO_PA(allockernelpage());
            debugf("map halt %d, va:%p, pa:%p", i, va, newpg);
            kvmmap(kpgtbl, va, newpg, PGSIZE, PTE_A | PTE_D | PTE_R | PTE_W);
        }
        c->sched_kstack_top = sched_stack + KERNEL_STACK_SIZE;
        // double the sched_stack to make a significant gap between different cpus.
        //  if any kernel stack overflows, it will page fault.
        sched_stack += 2 * KERNEL_STACK_SIZE;
    }

    // Step.5 : Kernel Direct Mapping

    // RISC-V CPU maps DDR starting at 0x8000_0000
    const uint64 physical_mems = PHYS_MEM_SIZE;
    int64 available_mems       = physical_mems - (kernel_image_end_2M - RISCV_DDR_BASE);
    if (available_mems <= 0)
        panic("No available memory for kernel direct mapping");
    infof("Memory after kernel image (phys) size = %p", available_mems);

    // map remaining pages to KERNEL_DIRECT_MAPPING_BASE + pa
    uint64 direct_mapping_vaddr = KERNEL_DIRECT_MAPPING_BASE + kernel_image_end_2M;
    uint64 direct_mapping_paddr = kernel_image_end_2M;
    kvmmap(kpgtbl, direct_mapping_vaddr, direct_mapping_paddr, available_mems, PTE_A | PTE_D | PTE_R | PTE_W);

    // We have allocated some pages from 0xffff_ffc0_802x_xxxx;
    // So page allocator should starts after these used pages.
    kpage_allocator_base = init_page_allocator;
    kpage_allocator_size = available_mems - (init_page_allocator - init_page_allocator_base);

#ifdef LOG_LEVEL_DEBUG
    vm_print(kpgtbl);
#endif

    // VisionFive2 Notes:
    // 	if PTE_A is not set here, it will trigger an instruction page fault scause 0xc for the first time-accesses.
    //		Then the trap-handler traps itself.
    //		Because page fault handler should handle the PTE_A and PTE_D bits in VF2
    //		QEMU works without PTE_A here.
    //	see: https://www.reddit.com/r/RISCV/comments/14psii6/comment/jqmad6g
    //	docs: Volume II: RISC-V Privileged Architectures V1.10, Page 61,
    //		> Two schemes to manage the A and D bits are permitted:
    // 			- ..., the implementation sets the corresponding bit in the PTE.
    //			- ..., a page-fault exception is raised.
    //		> Standard supervisor software should be written to assume either or both PTE update schemes may be in effect.

    return kpgtbl;
}

// Add a mapping to the kernel page table.
// only used when booting.
void kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
    assert(PGALIGNED(va));
    assert(PGALIGNED(pa));
    assert(PGALIGNED(sz));

    debugf("va:%p, pa:%p, sz:%x", va, pa, sz);

    pagetable_t __kva pgtbl_level1, pgtbl_level0;
    uint64 vpn2, vpn1, vpn0;

    uint64 __kva vaddr     = va;
    uint64 __kva paddr     = pa;
    uint64 __kva vaddr_end = vaddr + sz;

    while (vaddr < vaddr_end) {
        // try to add mapping: vaddr -> pa
        vpn2 = PX(2, vaddr);
        vpn1 = PX(1, vaddr);
        vpn0 = PX(0, vaddr);

        if (!(kpgtbl[vpn2] & PTE_V)) {
            // kpgtbl[vpn2] is not a valid PTE, allocate the level 1 pagetable.
            uint64 __kva newpg = allockernelpage();
            memset((void *)newpg, 0, PGSIZE);
            pgtbl_level1 = (pagetable_t)newpg;
            kpgtbl[vpn2] = MAKE_PTE(KVA_TO_PA(newpg), 0);
        } else {
            pte_t pte = kpgtbl[vpn2];
            // check validity: pte must points to next level page table.
            if ((pte & PTE_R) || (pte & PTE_W) || (pte & PTE_X))
                panic("kvmmap: vaddr %p already mapped at level 2", vaddr);
            pgtbl_level1 = (pagetable_t)PA_TO_KVA(PTE2PA(kpgtbl[vpn2]));
        }
        if (!(pgtbl_level1[vpn1] & PTE_V)) {
            // pgtbl_level1[vpn1] is not a valid PTE.
            //   try to allocate 2M page
            //   , or allocate the level 1 pagetable.
            if (IS_ALIGNED(vaddr, PGSIZE_2M) && IS_ALIGNED(paddr, PGSIZE_2M) && sz >= PGSIZE_2M) {
                // it's ok for a huge page.
                pgtbl_level1[vpn1] = MAKE_PTE(paddr, perm);
                vaddr += PGSIZE_2M;
                paddr += PGSIZE_2M;
                sz -= PGSIZE_2M;
                continue;
            }
            uint64 __kva newpg = allockernelpage();
            memset((void *)newpg, 0, PGSIZE);
            pgtbl_level0       = (pagetable_t)newpg;
            pgtbl_level1[vpn1] = MAKE_PTE(KVA_TO_PA(newpg), 0);
        } else {
            pte_t pte = pgtbl_level1[vpn1];
            // check validity: pte must points to next level page table.
            if ((pte & PTE_R) || (pte & PTE_W) || (pte & PTE_X))
                panic("kvmmap: vaddr %p already mapped at level 1", vaddr);
            pgtbl_level0 = (pagetable_t)PA_TO_KVA(PTE2PA(pgtbl_level1[vpn1]));
        }
        // check validity: pte must points to next level page table.
        if (pgtbl_level0[vpn0] & PTE_V)
            panic("kvmmap: vaddr %p already mapped at level 0", vaddr);
        pgtbl_level0[vpn0] = MAKE_PTE(paddr, perm);
        vaddr += PGSIZE;
        paddr += PGSIZE;
        sz -= PGSIZE;
    }
    assert(vaddr == vaddr_end);
    assert(sz == 0);
}