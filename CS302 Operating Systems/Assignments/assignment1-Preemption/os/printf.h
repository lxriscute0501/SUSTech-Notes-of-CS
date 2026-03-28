#ifndef PRINTF_H
#define PRINTF_H

void printf(char *fmy, ...);
__attribute__((noreturn)) void __panic(char *fmt, ...);

#endif  // PRINTF_H
