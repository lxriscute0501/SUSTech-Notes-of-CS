#include "defs.h"

void synclab_main(uint64);
void checkpoint1(void);
void checkpoint2(void);
void checkpoint3(void);

void synclab_init(void) {
    infof("sync lab init");
    create_kthread(synclab_main, 0);
}

void synclab_main(uint64) {
    checkpoint1();
    checkpoint2();
    checkpoint3();

    infof("all checks passed");
    exit(0);
}

