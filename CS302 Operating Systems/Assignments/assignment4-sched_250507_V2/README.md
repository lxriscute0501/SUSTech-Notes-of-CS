# xv6-sustech

A modified xv6 for SUSTech OS.

对标 [xv6-riscv](https://github.com/mit-pdos/xv6-riscv)。

docs (TODO): https://yuk1i.github.io/os-next-docs/

## 对比 xv6 改了什么

- S-mode 启动，OpenSBI 作为 M mode。
- 有 SMP，使用 SBI HSM Extension 启动多核心。
  - `make run` 启动单核，`make runsmp` 启动多核心。
  - sbi_hsm_hart_start 在单核情况下返回错误。
- 内核页表使用类似 Linux 的 Memory Layout，内核代码处于高地址，显著区分用户地址和内核地址。
  - 将内核地址划分为多个区域：
	- Kernel Image
	- Kernel Direct Mapping pages, 由 kallocpage/kfreepage 管理的页面分配。
	- Kernel Fixed-Size Object Heap, 由 kalloc/kfree 管理的固定大小对象分配池。
- 使用单独的 mm 和 vma 结构体管理用户空间内存。
- 使用类似 slab-allocator (kmem_cache) 动态分配固定大小的对象。
- 兼容 VisionFive 2 (JH7110) 开发板

## branches:

- yuki: 主分支
- nommu: 无页表，有 SMP，有 Kernel Thread + Scheduling
- fs: 带文件系统
- smp (deprecated): 第一次实现 smp 的原始分支

## User Prog

```shell
git clone git@github.com:yuk1i/SUSTechOS-2025S-user.git user

# clean:
make -C user clean

# make user
make user
```

## Boot log

```
OpenSBI v1.5
   ____                    _____ ____ _____
  / __ \                  / ____|  _ \_   _|
 | |  | |_ __   ___ _ __ | (___ | |_) || |
 | |  | | '_ \ / _ \ '_ \ \___ \|  _ < | |
 | |__| | |_) |  __/ | | |____) | |_) || |_
  \____/| .__/ \___|_| |_|_____/|____/_____|
        | |
        |_|

Platform Name             : riscv-virtio,qemu
Platform Features         : medeleg
Platform HART Count       : 4
Platform IPI Device       : aclint-mswi
Platform Timer Device     : aclint-mtimer @ 10000000Hz
Platform Console Device   : uart8250
Platform HSM Device       : ---
Platform PMU Device       : ---
Platform Reboot Device    : syscon-reboot
Platform Shutdown Device  : syscon-poweroff
Platform Suspend Device   : ---
Platform CPPC Device      : ---
Firmware Base             : 0x80000000
Firmware Size             : 357 KB
Firmware RW Offset        : 0x40000
Firmware RW Size          : 101 KB
Firmware Heap Offset      : 0x4f000
Firmware Heap Size        : 41 KB (total), 2 KB (reserved), 11 KB (used), 27 KB (free)
Firmware Scratch Size     : 4096 B (total), 416 B (used), 3680 B (free)
Runtime SBI Version       : 2.0

Domain0 Name              : root
Domain0 Boot HART         : 3
Domain0 HARTs             : 0*,1*,2*,3*
Domain0 Region00          : 0x0000000000100000-0x0000000000100fff M: (I,R,W) S/U: (R,W)
Domain0 Region01          : 0x0000000010000000-0x0000000010000fff M: (I,R,W) S/U: (R,W)
Domain0 Region02          : 0x0000000002000000-0x000000000200ffff M: (I,R,W) S/U: ()
Domain0 Region03          : 0x0000000080040000-0x000000008005ffff M: (R,W) S/U: ()
Domain0 Region04          : 0x0000000080000000-0x000000008003ffff M: (R,X) S/U: ()
Domain0 Region05          : 0x000000000c400000-0x000000000c5fffff M: (I,R,W) S/U: (R,W)
Domain0 Region06          : 0x000000000c000000-0x000000000c3fffff M: (I,R,W) S/U: (R,W)
Domain0 Region07          : 0x0000000000000000-0xffffffffffffffff M: () S/U: (R,W,X)
Domain0 Next Address      : 0x0000000080200000
Domain0 Next Arg1         : 0x000000009fe00000
Domain0 Next Mode         : S-mode
Domain0 SysReset          : yes
Domain0 SysSuspend        : yes

Boot HART ID              : 3
Boot HART Domain          : root
Boot HART Priv Version    : v1.12
Boot HART Base ISA        : rv64imafdch
Boot HART ISA Extensions  : sstc,zicntr,zihpm,zicboz,zicbom,sdtrig
Boot HART PMP Count       : 16
Boot HART PMP Granularity : 2 bits
Boot HART PMP Address Bits: 54
Boot HART MHPM Info       : 16 (0x0007fff8)
Boot HART Debug Triggers  : 2 triggers
Boot HART MIDELEG         : 0x0000000000001666
Boot HART MEDELEG         : 0x0000000000f0b509


=====
Hello World!
=====

Boot stack: 0x00000000802aa000
clean bss: 0x00000000802ab000 - 0x00000000802b4000
Boot m_hartid 3
[INFO 0] bootcpu_entry: basic smp inited, thread_id available now, we are cpu 3: 0x00000000802b30d8
Kernel Starts Relocating...
Kernel size: 0x00000000000b4000, Rounded to 2MiB: 0x0000000000200000
[INFO 0] bootcpu_start_relocation: Kernel phy_base: 0x0000000080200000, phy_end_4k:0x00000000802b4000, phy_end_2M 0x0000000080400000
Mapping Identity: 0x0000000080200000 to 0x0000000080200000
Mapping kernel image: 0xffffffff80200000 to 0x0000000080200000
Mapping Direct Mapping: 0xffffffc080400000 to 0x0000000080400000
Enable SATP on temporary pagetable.
Boot HART Relocated. We are at high address now! PC: 0xffffffff80202e04
[INFO 0] kvm_init: setup basic page allocator: base 0xffffffc080400000, end 0xffffffc080600000
[INFO 0] kvmmake: Memory after kernel image (phys) size = 0x0000000003c00000
[INFO 0] kvm_init: enable pageing at 0x8000000000080400
Relocated. Boot halt sp at 0xffffffffff001fa0
Boot another cpus.
- booting hart 0: hsm_hart_start(hartid=0, pc=_entry_sec, opaque=1) = 0. waiting for hart online
cpu 1 (halt 0) booting. Relocating
cpu 1 (halt 0) booted. sp: 0xffffffffff005fe0
- booting hart 1: hsm_hart_start(hartid=1, pc=_entry_sec, opaque=2) = 0. waiting for hart online
cpu 2 (halt 1) booting. Relocating
cpu 2 (halt 1) booted. sp: 0xffffffffff009fe0
- booting hart 2: hsm_hart_start(hartid=2, pc=_entry_sec, opaque=3) = 0. waiting for hart online
cpu 3 (halt 2) booting. Relocating
cpu 3 (halt 2) booted. sp: 0xffffffffff00dfe0
System has 4 cpus online

UART inited.
[INFO 0] kpgmgrinit: page allocator init: base: 0xffffffc080411000, stop: 0xffffffc084000000
[INFO 0] allocator_init: allocator mm inited base 0xfffffffd00000000
[INFO 0] allocator_init: allocator vma inited base 0xfffffffd01000000
[INFO 0] allocator_init: allocator proc inited base 0xfffffffd02000000
applist:
        ch2b_exit
        ch2b_hello_world
        ch2b_power
        ch3b_sleep
        ch3b_sleep1
        ch3b_yield0
        ch3b_yield1
        ch3b_yield2
        ch5b_exec_simple
        ch5b_exit
        ch5b_forktest0
        ch5b_forktest1
        ch5b_forktest2
        ch5b_getpid
        ch5b_usertest
        ch6b_args
        ch6b_assert
        ch6b_cat
        ch6b_exec
        ch6b_filetest
        ch6b_filetest_simple
        ch6b_panic
        ch6b_usertest
        ch8_mut1_deadlock
        ch8_sem1_deadlock
        ch8_sem2_deadlock
        ch8_usertest
        ch8b_mpsc_sem
        ch8b_mut_phi_din
        ch8b_mut_race
        ch8b_spin_mut_race
        ch8b_sync_sem
        ch8b_test_condvar
        ch8b_threads
        ch8b_threads_arg
        ch8b_usertest
        usershell
[INFO 0] load_init_app: load init proc usershell
[INFO 0] bootcpu_init: start scheduler!
[INFO 1] secondarycpu_init: start scheduler!
[INFO 3] secondarycpu_init: start scheduler!
[INFO 2] secondarycpu_init: start scheduler!
[INFO 0] scheduler: switch to proc 0(1)
C user shell
> 
```

## 本地开发测试

在本地开发并测试时，需要拉取 uCore-Tutorial-Test-2022A 到 `user` 文件夹。你可以根据网络情况和个人偏好选择下列一项执行：

```bash
# 清华 git 使用 https
git clone https://git.tsinghua.edu.cn/os-lab/public/ucore-tutorial-test-2022a.git user
# 清华 git 使用 ssh
git clone git@git.tsinghua.edu.cn:os-lab/public/ucore-tutorial-test-2022a.git user
# GitHub 使用 https
git clone https://github.com/LearningOS/uCore-Tutorial-Test-2022A.git user
# GitHub 使用 ssh
git clone git@github.com:LearningOS/uCore-Tutorial-Test-2022A.git user
```

注意：`user` 已添加至 `.gitignore`，你无需将其提交，ci 也不会使用它]


## uCore for VisionFive2

This repo contains my fixes to uCore-Tutorial-Code for running it on a VisionFive2 board.

See VisionFive2 Notes in codes.

### Checklist

- [done] pagetable 
- [done] interrupt
- [done] userspace
- [done] SMP

### gdb & openocd

This configuration uses a jlink. 

- https://github.com/starfive-tech/edk2/wiki/How-to-flash-and-debug-with-JTAG#connect-to-visionfive-2
- https://dram.page/p/visionfive-jtag-1/

clone https://github.com/riscv-collab/riscv-openocd

configure with: ` ./configure  --enable-verbose --prefix /data/os-riscv/vf2-debug --enable-ftdi  --enable-stlink --enable-ft232r   --enable-ftdi-cjtag  --enable-jtag_vpi --enable-jtag_dpi --enable-openjtag --enable-jlink --enable-cmsis-dap --enable-nulink`

`make && make install`

openocd conf:

```
gdb_memory_map disable

# JTAG adapter setup
adapter driver jlink
# adapter driver cmsis-dap
# use cmsis-dap for muselab's nanoDAP debugger

adapter speed 20000
transport select jtag

set _CHIPNAME riscv
jtag newtap $_CHIPNAME cpu0 -irlen 5
jtag newtap $_CHIPNAME cpu1 -irlen 5
set _TARGETNAME_1 $_CHIPNAME.cpu1
set _TARGETNAME_2 $_CHIPNAME.cpu2
set _TARGETNAME_3 $_CHIPNAME.cpu3
set _TARGETNAME_4 $_CHIPNAME.cpu4

target create $_TARGETNAME_1 riscv -chain-position $_CHIPNAME.cpu1 -coreid 1 -rtos hwthread
#target create $_TARGETNAME_2 riscv -chain-position $_CHIPNAME.cpu1 -coreid 2
#target create $_TARGETNAME_3 riscv -chain-position $_CHIPNAME.cpu1 -coreid 3
#target create $_TARGETNAME_4 riscv -chain-position $_CHIPNAME.cpu1 -coreid 4

#target smp $_TARGETNAME_1 $_TARGETNAME_2 $_TARGETNAME_3 $_TARGETNAME_4

# we only debug one core.

init
```

gdbinit:

```
set confirm off
source /data/os-riscv/gef/gef.py
set architecture riscv:rv64
file build/kernel
gef config context.show_registers_raw 1
#target remote 127.0.0.1:3333
gef-remote --qemu-user --qemu-binary build/kernel 127.0.0.1 3333
set riscv use-compressed-breakpoints yes
b *0x80200000
```

### PageTable PTE_A and PTE_D

```c
vm.c:kvmmake
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
```

为了使用 kerneltrap 处理 A/D bits 的 pagefault，需要对其做一定更改

```c
void kerneltrap() __attribute__((aligned(16)))
__attribute__((interrupt("supervisor")));	// gcc will generate codes for context saving and restoring.

void kerneltrap()
{
	if ((r_sstatus() & SSTATUS_SPP) == 0)
		panic("kerneltrap: not from supervisor mode");

	uint64 cause = r_scause();
	if (cause & (1ULL << 63)) {
		panic("kerneltrap enter with interrupt scause");
	}
	uint64 addr = r_stval();
	pagetable_t pgt = SATP_TO_PGTABLE(r_satp());
	pte_t *pte = walk(pgt, addr, 0);
	if (pte == NULL)
		panic("kernel pagefault at %p", addr);
	switch (cause) {
	case InstructionPageFault:
	case LoadPageFault:
		*pte |= PTE_A;
		break;
	case StorePageFault:
		*pte |= PTE_A | PTE_D;
		break;

	default:
		panic("trap from kernel");
	}
}
```

相关的ISA： svadu, https://github.com/riscvarchive/riscv-svadu, 已经被合并进 Privileged Specification.

VisionFive2 没有处理 PTE 中 A/D bits 的硬件，需要使用 pagefault 来处理。而 QEMU 则实现了 svadu ，可以直接设置 A/D bits.

为了保持 QEMU 和 VisionFive2 行为一致性方便调试，QEMU 中 svadu 可以通过 cpu flags 关掉：

```
QEMUOPTS = \
	-nographic \
	-machine virt \
	-cpu rv64,svadu=off \
	-kernel build/kernel	\
```

挂上 gdb 后通过 `info register menvcfg` 确认：

```
(qemu) gef➤  i r menvcfg 
menvcfg        0x80000000000000f0       0x80000000000000f0
```

注1：menvcfg 是 rv-privileged spec v1.12 的东西，最新的 spec 文档需要在 https://github.com/riscv/riscv-isa-manual/releases/ 中下载。
目前 "Privileged Specification version 20211203" 中并没有 menvcfg 的 ADUE 定义。

注2：QEMU 中相关的源代码：

target/riscv/cpu_helper.c: get_physical_address:

```c
    bool svade = riscv_cpu_cfg(env)->ext_svade;
    bool svadu = riscv_cpu_cfg(env)->ext_svadu;
    bool adue = svadu ? env->menvcfg & MENVCFG_ADUE : !svade;

    /*
     * If ADUE is enabled, set accessed and dirty bits.
     * Otherwise raise an exception if necessary.
     */
    if (adue) {
        updated_pte |= PTE_A | (access_type == MMU_DATA_STORE ? PTE_D : 0);
    } else if (!(pte & PTE_A) ||
               (access_type == MMU_DATA_STORE && !(pte & PTE_D))) {
        return TRANSLATE_FAIL;
    }
```

target/riscv/cpu_bits.h:

```c
/* Execution environment configuration bits */
#define MENVCFG_FIOM                       BIT(0)
#define MENVCFG_CBIE                       (3UL << 4)
#define MENVCFG_CBCFE                      BIT(6)
#define MENVCFG_CBZE                       BIT(7)
#define MENVCFG_ADUE                       (1ULL << 61)
#define MENVCFG_PBMTE                      (1ULL << 62)
#define MENVCFG_STCE                       (1ULL << 63)
```