#include "timer.h"

#include "riscv.h"
#include "sbi.h"

/// read the `mtime` regiser
uint64 get_cycle() {
    return r_time();
}

/// Enable timer interrupt
void timer_init() {
    // Enable supervisor timer interrupt
    w_sie(r_sie() | SIE_STIE);
    set_next_timer();
}

// /// Set the next timer interrupt
void set_next_timer() {
    const uint64 timebase = CPU_FREQ / TICKS_PER_SEC;
    set_timer(get_cycle() + timebase);
}

void sleepms(uint64 ms) {
    uint64 now = r_time();
    uint64 diff = ms * CPU_FREQ / 1000;
    while (r_time() - now < diff || r_time() < now) {
        asm volatile("":::"memory");
    }
}