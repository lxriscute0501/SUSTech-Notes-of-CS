#include "console.h"

#include "debug.h"
#include "defs.h"
#include "sbi.h"

static int uart_inited = false;
static void uart_putchar(int);

static struct spinlock uart_tx_lock;
volatile int panicked = 0;

#define BACKSPACE 0x100
#define C(x)      ((x) - '@')  // Control-x

struct {
    spinlock_t lock;

    // input
#define INPUT_BUF_SIZE 128
    char buf[INPUT_BUF_SIZE];
    uint r;  // Read index
    uint w;  // Write index
    uint e;  // Edit index
} cons;

void consputc(int c) {
    if (!uart_inited || panicked)  // when panicked, use SBI output
        sbi_putchar(c);
    else {
        if (c == BACKSPACE) {
            uart_putchar('\b');
            uart_putchar(' ');
            uart_putchar('\b');
        } else {
            uart_putchar(c);
        }
    }
}

static void uart_putchar(int ch) {
    int intr = intr_off();
    while ((ReadReg(LSR) & LSR_TX_IDLE) == 0) MEMORY_FENCE();
    MEMORY_FENCE();

    WriteReg(THR, ch);
    MEMORY_FENCE();
    if (intr)
        intr_on();
}

void console_init() {
    assert(!uart_inited);
    spinlock_init(&uart_tx_lock, "uart_tx");
    spinlock_init(&cons.lock, "cons");

    // disable interrupts.
    WriteReg(IER, 0x00);
    MEMORY_FENCE();

    // special mode to set baud rate.
    WriteReg(LCR, LCR_BAUD_LATCH);
    MEMORY_FENCE();

    // LSB for baud rate of 38.4K.
    WriteReg(0, 0x03);
    MEMORY_FENCE();

    // MSB for baud rate of 38.4K.
    WriteReg(1, 0x00);
    MEMORY_FENCE();
    // leave set-baud mode,

    // and set word length to 8 bits, no parity.
    WriteReg(LCR, LCR_EIGHT_BITS);
    MEMORY_FENCE();

    // reset and enable FIFOs.
    WriteReg(FCR, FCR_FIFO_ENABLE | FCR_FIFO_CLEAR);
    MEMORY_FENCE();

    // enable receive interrupts.
    WriteReg(IER, IER_RX_ENABLE);
    MEMORY_FENCE();
    uart_inited = true;
}

static void consintr(int c) {
    acquire(&cons.lock);

    switch (c) {
        case C('P'):
            print_procs();
            break;
        case C('Q'):
            print_kpgmgr();
            break;
        case C('U'):  // Kill line.
            while (cons.e != cons.w && cons.buf[(cons.e - 1) % INPUT_BUF_SIZE] != '\n') {
                cons.e--;
                consputc(BACKSPACE);
            }
            break;
        case '\x7f':  // Delete key
            if (cons.e != cons.w) {
                cons.e--;
                consputc(BACKSPACE);
            }
            break;
        default:
            if (c != 0 && cons.e - cons.r < INPUT_BUF_SIZE) {
                c = (c == '\r') ? '\n' : c;

                // echo back to the user.
                consputc(c);

                // store for consumption by consoleread().
                cons.buf[cons.e++ % INPUT_BUF_SIZE] = c;

                if (c == '\n' || c == C('D') || cons.e - cons.r == INPUT_BUF_SIZE) {
                    // wake up consoleread() if a whole line (or end-of-file)
                    // has arrived.
                    cons.w = cons.e;
                    wakeup(&cons);
                }
            }
            break;
    }

    release(&cons.lock);
}

static int uartgetc(void) {
    if (ReadReg(LSR) & 0x01) {
        // input data is ready.
        return ReadReg(RHR);
    } else {
        return -1;
    }
}

void uart_intr() {
    while (1) {
        int c = uartgetc();
        if (c == -1)
            break;
        // infof("uart: %c", c);
        consintr(c);
    }
}

