#ifndef LOG_H
#define LOG_H

#include "printf.h"
#include "proc.h"

extern void dummy(int, ...);
extern void shutdown() __attribute__((noreturn));

// debug: force trace level
#define LOG_LEVEL_INFO

#if defined(LOG_LEVEL_ERROR)

#define USE_LOG_ERROR

#endif  // LOG_LEVEL_ERROR

#if defined(LOG_LEVEL_WARN)

#define USE_LOG_ERROR
#define USE_LOG_WARN

#endif  // LOG_LEVEL_ERROR

#if defined(LOG_LEVEL_INFO)

#define USE_LOG_ERROR
#define USE_LOG_WARN
#define USE_LOG_INFO

#endif  // LOG_LEVEL_INFO

#if defined(LOG_LEVEL_DEBUG)

#define USE_LOG_ERROR
#define USE_LOG_WARN
#define USE_LOG_INFO
#define USE_LOG_DEBUG

#endif  // LOG_LEVEL_DEBUG

#if defined(LOG_LEVEL_TRACE)

#define USE_LOG_ERROR
#define USE_LOG_WARN
#define USE_LOG_INFO
#define USE_LOG_DEBUG
#define USE_LOG_TRACE

#endif  // LOG_LEVEL_TRACE

static inline int __safe_pid() {
    struct proc *p = curr_proc();
    return p ? p->pid : -1;
}

enum LOG_COLOR {
    RED = 31,
    GREEN = 32,
    BLUE = 34,
    PURPLE = 35,
    CYAN = 36,
    GRAY = 90,
    YELLOW = 93,
};

#if defined(USE_LOG_ERROR)
#define errorf(fmt, ...)                                                                    do {                                               \
        printf("\x1b[%dm[%s %d,%d] %s: " fmt "\x1b[0m\n", \
               RED,                                 \
               "ERROR",                                 \
               cpuid(),                                \
               __safe_pid(),                           \
               __func__,                               \
               ##__VA_ARGS__);                         \
    } while (0)
#else
#define errorf(fmt, ...) dummy(0, ##__VA_ARGS__)
#endif  // USE_LOG_ERROR

#if defined(USE_LOG_WARN)
#define warnf(fmt, ...)                                \
    do {                                               \
        printf("\x1b[%dm[%s %d,%d] %s: " fmt "\x1b[0m\n", \
               YELLOW,                                 \
               "WARN ",                                 \
               cpuid(),                                \
               __safe_pid(),                           \
               __func__,                               \
               ##__VA_ARGS__);                         \
    } while (0)
#else
#define warnf(fmt, ...) dummy(0, ##__VA_ARGS__)
#endif  // USE_LOG_WARN

#if defined(USE_LOG_INFO)
#define infof(fmt, ...)                                \
    do {                                               \
        printf("\x1b[%dm[%s %d,%d] %s: " fmt "\x1b[0m\n", \
               BLUE,                                   \
               "INFO ",                                 \
               cpuid(),                                \
               __safe_pid(),                           \
               __func__,                               \
               ##__VA_ARGS__);                         \
    } while (0)
#else
#define infof(fmt, ...) dummy(0, ##__VA_ARGS__)
#endif  // USE_LOG_INFO

#if defined(USE_LOG_DEBUG)
#define debugf(fmt, ...)                               \
    do {                                               \
        printf("\x1b[%dm[%s %d,%d] %s: " fmt "\x1b[0m\n", \
               GREEN,                                  \
               "DEBUG",                                \
               cpuid(),                                \
               __safe_pid(),                           \
               __func__,                               \
               ##__VA_ARGS__);                         \
    } while (0)
#else
#define debugf(fmt, ...) dummy(0, ##__VA_ARGS__)
#endif  // USE_LOG_DEBUG

#if defined(USE_LOG_TRACE)
#define tracef(fmt, ...)                                                                  \
    do {                                                                                  \
        printf("\x1b[%dm[%s %d,%d]" fmt "\x1b[0m\n", GRAY, "TRACE", cpuid(),__safe_pid(), ##__VA_ARGS__); \
    } while (0)
#else
#define tracef(fmt, ...) dummy(0, ##__VA_ARGS__)
#endif  // USE_LOG_TRACE

// User-defined log color

#define logf(color, text, fmt, ...)                           \
    do {                                               \
        printf("\x1b[%dm[%s %d,%d] %s: " fmt "\x1b[0m\n", \
               color,                                  \
               text,                                \
               cpuid(),                                \
               __safe_pid(),                           \
               __func__,                               \
               ##__VA_ARGS__);                         \
    } while (0)

#define panic(fmt, ...)                                    \
    do {                                                   \
        __panic("\x1b[%dm[%s %d,%d] %s:%d: " fmt "\x1b[0m\n", \
                RED,                                       \
                "PANIC",                                   \
                cpuid(),                                   \
                __safe_pid(),                              \
                __FILE__,                                  \
                __LINE__,                                  \
                ##__VA_ARGS__);                            \
    } while (0)

#define panic_never_reach() panic("should never reach here")

#define assert(x)                              \
    do {                                       \
        if (!(x))                              \
            panic("assertion failed: %s", #x); \
    } while (0)

#define assert_str(x, fmt, ...)                                     \
    do {                                                            \
        if (!(x))                                                   \
            panic("assertion failed: %s, " fmt, #x, ##__VA_ARGS__); \
    } while (0)

#define assert_equals(expecteds, actuals, fmt, ...)                               \
    do {                                                                          \
        typeof(expecteds) _exp = (expecteds);                                     \
        typeof(actuals) _act = (actuals);                                         \
        if (_exp != _act) {                                                       \
            panic("expectation failed: %s != %s, expected: %lx actuals: %lx" fmt, \
                  #expecteds,                                                     \
                  #actuals,                                                       \
                  _exp,                                                           \
                  _act,                                                           \
                  ##__VA_ARGS__);                                                 \
        }                                                                         \
    } while (0)

#define static_assert(expr, ...) __static_assert(expr, ##__VA_ARGS__, #expr)
#define __static_assert(expr, msg, ...) _Static_assert(expr, msg)

#endif  //! LOG_H
