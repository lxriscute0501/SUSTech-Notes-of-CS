#include "defs.h"
#include "log.h"
#include "proc.h"
#include "string.h"

static struct cpu cpus[NCPU];

struct cpu* mycpu() {
    assert(!intr_get());
    int id = cpuid();
    assert(id >= 0 && id < NCPU);
    return &cpus[id];
}

struct cpu* getcpu(int i) {
    assert(i >= 0 && i < NCPU);
    return &cpus[i];
}