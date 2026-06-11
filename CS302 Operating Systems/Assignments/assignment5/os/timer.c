#include "timer.h"

#include "riscv.h"
#include "sbi.h"

extern int on_vf2_board;

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
    if (on_vf2_board) {
        set_timer(get_cycle() + timebase);
    } else {
        w_stimecmp(r_time() + timebase);
    }
}