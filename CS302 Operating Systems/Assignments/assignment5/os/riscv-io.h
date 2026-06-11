#ifndef __RISC_V_IO_H__
#define __RISC_V_IO_H__

#include "types.h"
/* clang-format off */

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;

static inline u8 __raw_readb(const volatile void *addr)
{
	u8 val;

	asm volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u16 __raw_readw(const volatile void *addr)
{
	u16 val;

	asm volatile("lh %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u32 __raw_readl(const volatile void *addr)
{
	u32 val;

	asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline void __raw_writeb(u8 val, volatile void *addr)
{
	asm volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writew(u16 val, volatile void *addr)
{
	asm volatile("sh %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writel(u32 val, volatile void *addr)
{
	asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}

#define __io_br()	do {} while (0)
#define __io_ar()	__asm__ __volatile__ ("fence i,r" : : : "memory");
#define __io_bw()	__asm__ __volatile__ ("fence w,o" : : : "memory");
#define __io_aw()	do {} while (0)

#define readb(c)	({ uint8  __v; __io_br(); __v = __raw_readb(c); __io_ar(); __v; })
#define readl(c)	({ uint32 __v; __io_br(); __v = __raw_readl(c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(c)); __io_aw(); })

#endif  // __RISC_V_IO_H__