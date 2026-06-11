#include "loader.h"

#include "defs.h"
#include "elf.h"
#include "trap.h"

// Get user progs' infomation through pre-defined symbol in `link_app.S`
void loader_init() {
    printf("applist:\n");
    for (struct user_app *app = user_apps; app->name != NULL; app++) {
        printf("\t%s\n", app->name);
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)app->elf_address;
        assert_str(ehdr->e_ident[0] == 0x7F && ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' && ehdr->e_ident[3] == 'F', "invalid elf header: %s", app->name);
        assert_equals(ehdr->e_phentsize, sizeof(Elf64_Phdr), "invalid program header size");
    }
}

struct user_app *get_elf(char *name) {
    if (strlen(name) == 0)
        return NULL;
    for (struct user_app *app = user_apps; app->name != NULL; app++) {
        if (strncmp(name, app->name, strlen(name)) == 0)
            return app;
    }
    return NULL;
}

int load_user_elf(struct user_app *app, struct proc *p, char *args[]) {
    if (p == NULL || p->state == UNUSED)
        panic("...");
    acquire(&p->mm->lock);
    int ret;

    Elf64_Ehdr *ehdr      = (Elf64_Ehdr *)app->elf_address;
    Elf64_Phdr *phdr_base = (Elf64_Phdr *)(app->elf_address + ehdr->e_phoff);
    uint64 max_va_end     = 0;
    for (int i = 0; i < ehdr->e_phnum; i++) {
        Elf64_Phdr *phdr = &phdr_base[i];
        // we only load from PT_LOAD phdrs
        if (phdr->p_type != PT_LOAD)
            continue;
        assert_str(PGALIGNED(phdr->p_vaddr), "Simplified loader only support page-aligned p_vaddr: %p", phdr->p_vaddr);

        // resolve the permission of PTE for this phdr
        int pte_perm = PTE_U;
        if (phdr->p_flags & PF_R)
            pte_perm |= PTE_R;
        if (phdr->p_flags & PF_W)
            pte_perm |= PTE_W;
        if (phdr->p_flags & PF_X)
            pte_perm |= PTE_X;

        struct vma *vma = mm_create_vma(p->mm);
        vma->vm_start   = PGROUNDDOWN(phdr->p_vaddr);  // The ELF requests this phdr loaded to p_vaddr;
        vma->vm_end     = PGROUNDUP(vma->vm_start + phdr->p_memsz);
        vma->pte_flags  = pte_perm;

        if ((ret = mm_mappages(vma)) < 0) {
            errorf("mm_mappages");
            return ret;
        }

        int64 file_off      = 0;
        uint64 file_remains = phdr->p_filesz;

        for (uint64 va = vma->vm_start; va < vma->vm_end; va += PGSIZE) {
            void *__kva pa = (void *)PA_TO_KVA(walkaddr(p->mm, va));
            void *src      = (void *)(app->elf_address + phdr->p_offset + file_off);

            uint64 copy_size = MIN(file_remains, PGSIZE);
            memmove(pa, src, copy_size);

            if (copy_size < PGSIZE) {
                // clear remaining bytes
                memset(pa + copy_size, 0, PGSIZE - copy_size);
            }
            file_off += copy_size;
            file_remains -= copy_size;
        }

        if (phdr->p_memsz > phdr->p_filesz) {
            // ELF requests larger memory than filesz
            // 	these are .bss segment, clear it to zero.

            uint64 bss_start = phdr->p_vaddr + phdr->p_filesz;
            uint64 bss_end   = phdr->p_vaddr + phdr->p_memsz;

            for (uint64 va = bss_start; va < bss_end; va = PGROUNDDOWN(va) + PGSIZE) {
                // set [va, page boundary of ba) to zero
                uint64 page_off   = va - PGROUNDDOWN(va);
                uint64 clear_size = PGROUNDUP(va) - va;
                void *__kva pa    = (void *)PA_TO_KVA(walkaddr(p->mm, PGROUNDDOWN(va)));
                memset(pa + page_off, 0, clear_size);
            }
        }

        assert(file_remains == 0);
        max_va_end = MAX(max_va_end, PGROUNDUP(phdr->p_vaddr + phdr->p_memsz));
    }

    // setup brk: zero
    p->vma_brk            = mm_create_vma(p->mm);
    p->vma_brk->vm_start  = max_va_end;
    p->vma_brk->vm_end    = max_va_end;
    p->vma_brk->pte_flags = PTE_R | PTE_W | PTE_U;
    if ((ret = mm_mappages(p->vma_brk)) < 0) {
        errorf("mm_mappages vma_brk");
        return ret;
    }
    p->brk = max_va_end;

    // setup stack
    struct vma *vma_ustack = mm_create_vma(p->mm);
    vma_ustack->vm_start   = USTACK_START - USTACK_SIZE;
    vma_ustack->vm_end     = USTACK_START;
    vma_ustack->pte_flags  = PTE_R | PTE_W | PTE_U;
    if ((ret = mm_mappages(vma_ustack)) < 0) {
        errorf("mm_mappages ustack");
        return ret;
    }

    for (uint64 va = vma_ustack->vm_start; va < vma_ustack->vm_end; va += PGSIZE) {
        void *__kva pa = (void *)PA_TO_KVA(walkaddr(p->mm, va));
        memset(pa, 0, PGSIZE);
    }

    // push strings
    uint64 __user uargv[MAXARG];
    uint64 sp = USTACK_START;
    int len, argc = 0;
    for (int i = 0; args[i] != NULL; i++) {
        len       = strlen(args[i]) + 1;
        sp        = sp - len;
        sp        = sp & ~7;  // align to 8 bytes
        void *kva = (void *)PA_TO_KVA(useraddr(p->mm, sp));
        memmove(kva, args[i], len);
        uargv[i] = sp;  // save the start address of string to uargv
        argc++;
    }
    assert(IS_ALIGNED(sp, 8));

    // push argv array
    sp = sp - sizeof(uint64);
    // allocate a NULL
    for (int i = argc - 1; i >= 0; i--) {
        sp             = sp - sizeof(uint64);
        void *kva      = (void *)PA_TO_KVA(useraddr(p->mm, sp));
        *(uint64 *)kva = uargv[i];
    }
    uint64 uargv_ptr = sp;
    sp               = sp & ~15;  // aligned to 16 bytes
    assert(IS_ALIGNED(sp, 16));

    // setup trapframe
    p->trapframe->sp  = sp;
    p->trapframe->epc = ehdr->e_entry;
    p->trapframe->a0  = argc;
    p->trapframe->a1  = uargv_ptr;
    p->state          = RUNNABLE;

    release(&p->mm->lock);
    // vm_print(p->mm->pgt);

    return 0;
}

#define INIT_PROC "init"
extern struct proc *init_proc;  // defined in proc.c

// load all apps and init the corresponding `proc` structure.
int load_init_app() {
    struct user_app *app = get_elf(INIT_PROC);
    if (app == NULL) {
        panic("fail to lookup init elf %s", INIT_PROC);
    }

    struct proc *p = allocproc();
    if (p == NULL) {
        panic("allocproc");
    }
    infof("load init proc %s", INIT_PROC);

    char *argv[] = {NULL};
    if (load_user_elf(app, p, argv) < 0) {
        panic("fail to load init elf.");
    }
    add_task(p);
    init_proc = p;
    release(&p->lock);
    return 0;
}
