#include "console.h"
#include "debug.h"
#include "defs.h"
#include "kalloc.h"
#include "loader.h"
#include "plic.h"
#include "proc.h"
#include "sbi.h"
#include "timer.h"

uint64 __pa kernel_image_end_4k;
uint64 __pa kernel_image_end_2M;

static char relocate_pagetable[PGSIZE] __attribute__((aligned(PGSIZE)));
static char relocate_pagetable_level1_ident[PGSIZE] __attribute__((aligned(PGSIZE)));
static char relocate_pagetable_level1_direct_mapping[PGSIZE] __attribute__((aligned(PGSIZE)));
static char relocate_pagetable_level1_high[PGSIZE] __attribute__((aligned(PGSIZE)));

__noreturn static void bootcpu_start_relocation();
__noreturn static void bootcpu_relocating();
__noreturn void secondarycpu_entry(int mhartid, int cpuid);
__noreturn static void secondarycpu_relocating();

static void bootcpu_init();
static void secondarycpu_init();

static volatile int booted_count       = 0;
static volatile int halt_specific_init = 0;
int on_vf2_board = 0;

allocator_t kstrbuf;

/** Multiple CPU (SMP) Boot Process:
 * ------------
 * | Boot CPU |  cpuid = 0, m_hartid = random
 * ------------
 *      | OpenSBI
 * -------------------------
 * | _entry, bootcpu_entry |
 * -------------------------
 *      | sp <= boot_stack (PA)
 * ----------------------------
 * | bootcpu_start_relocation |
 * ----------------------------
 *      | satp <= relocate_pagetable
 *      | sp   <= boot_stack (KIVA)
 * ----------------------
 * | bootcpu_relocating |
 * ----------------------
 *      | kvm_init : setup kernel_pagetable
 *      |
 *      | satp <= kernel_pagetable
 *      | sp   <= percpu_sched_stack (KVA)
 * ----------------
 * | bootcpu_init |
 * ----------------
 *    |                                             ------------------------
 *    | OpenSBI: HSM_Hart_Start         ------->    | _entry_secondary_cpu |
 *    |                                             ------------------------
 *    |                                                     | sp <= boot_stack (PA)
 *    |                                             ----------------------
 *    |                                             | secondarycpu_entry |
 *    |                                             ----------------------
 *    |                                                     | satp <= relocate_pagetable
 *    |                                                     | sp   <= boot_stack (KIVA)
 *    |                                             ---------------------------
 *    |                                             | secondarycpu_relocating |
 *    |                                             ---------------------------
 *    |                                                     | satp <= kernel_pagetable
 *    |                                                     | sp   <= percpu_sched_stack (KVA)
 *    | wait for all cpu online                     ---------------------
 *    |                                             | secondarycpu_init |
 *    | platform level init :                       ---------------------
 *    |   console, plic, kpgmgr,                            | wait for `halt_specific_init`
 *    |   uvm, proc, loader                                 |
 *    |                                                     |
 *    | halt_init: trap, timer, plic_hart                   | halt_init: trap, timer, plic_hart
 *    |                                                     |
 * -------------                                    -------------
 * | scheduler |                                    | scheduler |
 * -------------                                    -------------
 */

void bootcpu_entry(int mhartid) {
    printf("\n\n=====\nHello World!\n=====\n\nBoot stack: %p\nclean bss: %p - %p\n", boot_stack, s_bss, e_bss);
    memset(s_bss, 0, e_bss - s_bss);

    uint64 vendor = sbi_get_mvendorid();
    uint64 impl = sbi_get_mimpid();
    if (vendor == 0x489 && impl == 0x4210427) {
        printf("=== Boot: Detect SiFive U74, Possible running on VisionFive 2 board ===\n");
        printf("Assume s-mode U-boot exists. mhartid is stored in tp.\n");
        on_vf2_board = 1;
        mhartid = r_tp();
    }

    printf("Boot m_hartid %d\n", mhartid);

    // the boot hart always has cpuid == 0
    w_tp(0);
    // after setup tp, we can use mycpu()
    mycpu()->cpuid    = 0;
    mycpu()->mhart_id = mhartid;

    // functions in log.h requres mycpu() to be initialized.

    infof("basic smp inited, thread_id available now, we are cpu %d: %p", mhartid, mycpu());

    printf("Kernel Starts Relocating...\n");
    bootcpu_start_relocation();

    // We will jump to kernel's real pagetable in relocation_start.
    __builtin_unreachable();
}

__noreturn static void bootcpu_relocating() {
    printf("Boot HART Relocated. We are at high address now! PC: %p\n", r_pc());

    // Step 4. Rebuild final kernel pagetable
    kvm_init();

    uint64 new_sp = mycpu()->sched_kstack_top;
    uint64 fn     = (uint64)&bootcpu_init;

    asm volatile(
        "mv a1, %0 \n \
        mv sp, %1 \n \
        jr a1" ::"r"(fn),"r"(new_sp));
    __builtin_unreachable();
}

__noreturn void secondarycpu_entry(int hartid, int cpuid) {
    printf("cpu %d (halt %d) booting. Relocating\n", cpuid, hartid);

    // init mycpu()
    w_tp(cpuid);
    getcpu(cpuid)->mhart_id = hartid;
    getcpu(cpuid)->cpuid    = cpuid;

    // switch to temporary pagetable.
    w_satp(MAKE_SATP(relocate_pagetable));
    sfence_vma();

    // jump to kernel's high address
    uint64 fn = (uint64)&secondarycpu_relocating + KERNEL_OFFSET;
    uint64 sp = (uint64)&boot_stack_top + KERNEL_OFFSET;

    asm volatile(
        "mv a1, %0 \n \
        mv sp, %1 \n \
        jr a1" ::"r"(fn),"r"(sp));
    __builtin_unreachable();
}

__noreturn static void secondarycpu_relocating() {
    w_satp(MAKE_SATP(KVA_TO_PA(kernel_pagetable)));

    uint64 sp = mycpu()->sched_kstack_top;
    uint64 fn = (uint64)&secondarycpu_init;

    asm volatile(
        "mv a1, %0 \n \
        mv sp, %1 \n \
        jr a1" ::"r"(fn),"r"(sp));
    __builtin_unreachable();
}

static void bootcpu_init() {
    printf("Relocated. Boot halt sp at %p\n", r_sp());

#ifdef ENABLE_SMP
    printf("Boot another cpus.\n");

    // Attention: OpenSBI does not guarantee the boot cpu has mhartid == 0.
    // We assume mhartid begins at 0, aka NCPU == the number of cpus in the system, although spec does not guarantee this.
    {
        int cpuid = 1;
        int max_hartid = NCPU;
        if (on_vf2_board) 
            max_hartid++;

        for (int hartid = 0; hartid < max_hartid; hartid++) {
            if (hartid == mycpu()->mhart_id)
                continue;
            if (on_vf2_board && hartid == 0)
                continue;   // skip for hart 0 for vf2 (jh7110), it's a S7 core instead of U74.

            int saved_booted_cnt = booted_count;

            printf("- booting hart %d: hsm_hart_start(hartid=%d, pc=_entry_sec, opaque=%d)", hartid, hartid, cpuid);
            int ret = sbi_hsm_hart_start(hartid, KIVA_TO_PA(_entry_secondary_cpu), cpuid);
            printf(" = %d.\n", ret);
            if (ret < 0) {
                printf("skipped for hart %d\n", hartid);
                continue;
            }
            while (booted_count == saved_booted_cnt);
            cpuid++;
        }
        printf("System has %d cpus online\n\n", cpuid);
    }
#endif

    memset(relocate_pagetable, 0xde, PGSIZE);
    memset(relocate_pagetable_level1_ident, 0xde, PGSIZE);
    memset(relocate_pagetable_level1_direct_mapping, 0xde, PGSIZE);
    memset(relocate_pagetable_level1_high, 0xde, PGSIZE);

    trap_init();
    console_init();
    printf("UART inited.\n");
    plicinit();
    kpgmgrinit();
    uvm_init();
    proc_init();
    allocator_init(&kstrbuf, "kstrbuf", KSTRING_MAX, 4096);
    loader_init();
    load_init_app();

    timer_init();
    plicinithart();

    MEMORY_FENCE();
    halt_specific_init = 1;
    MEMORY_FENCE();

    infof("start scheduler!");
    scheduler();

    panic("scheduler returns");
}

static void secondarycpu_init() {
    printf("cpu %d (halt %d) booted. sp: %p\n", mycpu()->cpuid, mycpu()->mhart_id, r_sp());
    booted_count++;
    while (!halt_specific_init);

    trap_init();
    timer_init();
    plicinithart();

    infof("start scheduler!");
    scheduler();

    panic("scheduler returns");
}

__noreturn static void bootcpu_start_relocation() {
    assert(IS_ALIGNED(KERNEL_PHYS_BASE, PGSIZE_2M));
    // Although the kernel is compiled against VMA 0xffffffff80200000,
    //  we are still running under the Physical Address 0x80200000.

    // Step. 1: Setup a temporary pagetable.
    memset(relocate_pagetable, 0, PGSIZE);
    memset(relocate_pagetable_level1_ident, 0, PGSIZE);
    memset(relocate_pagetable_level1_direct_mapping, 0, PGSIZE);
    memset(relocate_pagetable_level1_high, 0, PGSIZE);

    pagetable_t pgt_root    = (pagetable_t)relocate_pagetable;
    pagetable_t pgt_ident   = (pagetable_t)relocate_pagetable_level1_ident;
    pagetable_t pgt_direct  = (pagetable_t)relocate_pagetable_level1_direct_mapping;
    pagetable_t pgt_kernimg = (pagetable_t)relocate_pagetable_level1_high;

    // Calculate Kernel image size, and round up to 2MiB.
    uint64 kernel_size    = (uint64)ekernel - (uint64)skernel;
    uint64 kernel_size_4K = ROUNDUP_2N(kernel_size, PGSIZE);
    uint64 kernel_size_2M = ROUNDUP_2N(kernel_size, PGSIZE_2M);

    kernel_image_end_4k = KERNEL_PHYS_BASE + kernel_size_4K;
    kernel_image_end_2M = KERNEL_PHYS_BASE + kernel_size_2M;

    printf("Kernel size: %p, Rounded to 2MiB: %p\n", kernel_size, kernel_size_2M);

    // Calculate Kernel Mapping Base & End
    uint64 kernel_phys_base = KERNEL_PHYS_BASE;
    uint64 kernel_phys_end  = kernel_phys_base + kernel_size_2M;
    uint64 kernel_virt_base = KERNEL_VIRT_BASE;
    uint64 kernel_virt_end  = kernel_virt_base + kernel_size_2M;

    // Calculate the first Direct Mapping Base & End
    uint64 kernel_la_phy_base = kernel_image_end_2M;
    uint64 kernel_la_base     = KERNEL_DIRECT_MAPPING_BASE + kernel_la_phy_base;
    uint64 kernel_la_end      = kernel_la_base + PGSIZE_2M;

    infof("Kernel phy_base: %p, phy_end_4k:%p, phy_end_2M %p", kernel_phys_base, kernel_image_end_4k, kernel_phys_end);

    // We will still have some instructions executed on pc 0x8020xxxx before jumping to KIVA.
    // Step 2. Setup Identity Mapping for 0x80200000 -> 0x80200000, using 2MiB huge page.
    {
        uint64 VPN2    = PX(2, kernel_phys_base);
        pgt_root[VPN2] = MAKE_PTE((uint64)pgt_ident, 0);

        for (uint64 pa = kernel_phys_base; pa < kernel_phys_end; pa += PGSIZE_2M) {
            uint64 va       = pa;
            uint64 vpn1     = PX(1, va);
            pgt_ident[vpn1] = MAKE_PTE(pa, PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
            printf("Mapping Identity: %p to %p\n", va, pa);
        }
    }

    // Step 3. Setup Kernel Image Mapping at high address
    {
        uint64 vpn2    = PX(2, kernel_virt_base);
        pgt_root[vpn2] = MAKE_PTE((uint64)pgt_kernimg, 0);

        for (uint64 pa = kernel_phys_base; pa < kernel_phys_end; pa += PGSIZE_2M) {
            uint64 va         = pa + KERNEL_OFFSET;
            uint64 vpn1       = PX(1, va);
            pgt_kernimg[vpn1] = MAKE_PTE(pa, PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
            printf("Mapping kernel image: %p to %p\n", va, pa);
        }
    }

    // Step 4. Setup Kernel Direct Mapping (Partially)
    {
        // This Direct Mapping area is used in kvmmake.
        // Only map one 2MiB [kernel_la_base - kernel_la_end]
        uint64 vpn2      = PX(2, kernel_la_base);
        pgt_root[vpn2]   = MAKE_PTE((uint64)pgt_direct, 0);
        uint64 vpn1      = PX(1, kernel_la_base);
        pgt_direct[vpn1] = MAKE_PTE(kernel_la_phy_base, PTE_R | PTE_W | PTE_A | PTE_D);
        printf("Mapping Direct Mapping: %p to %p\n", kernel_la_base, kernel_la_phy_base);
    }

#ifdef LOG_LEVEL_DEBUG
    vm_print_tmp(pgt_root);
#endif

    // Step 4. Enable SATP and jump to higher VirtAddr.
    printf("Enable SATP on temporary pagetable.\n");
    w_satp(MAKE_SATP(pgt_root));
    sfence_vma();

    uint64 fn = (uint64)&bootcpu_relocating + KERNEL_OFFSET;
    uint64 sp = (uint64)&boot_stack_top + KERNEL_OFFSET;

    asm volatile(
        "mv a1, %0 \n \
        mv sp, %1 \n \
        jr a1" ::"r"(fn),"r"(sp));

    __builtin_unreachable();
}