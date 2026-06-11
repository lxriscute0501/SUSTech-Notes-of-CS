#include "vm.h"
#include "trap.h"


void print_trapframe(struct trapframe *tf);
void print_ktrapframe(struct ktrapframe *tf);
void print_sysregs(int explain);
void print_procs();
void print_kpgmgr();
void vm_print(pagetable_t __kva pagetable);
void vm_print_tmp(pagetable_t __pa pagetable);
void vm_print_u(pagetable_t pagetable);
void mm_print(struct mm* mm);
