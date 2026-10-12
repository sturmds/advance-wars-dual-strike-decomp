#ifndef DIVIDER_IO_H
#define DIVIDER_IO_H

#include "types.h"

/* ARM9 hardware divider registers. */
#define REG_DIVCNT (*(volatile u16*)0x04000280)
#define REG_DIV_NUMER (*(volatile s32*)0x04000290)
#define IO_DIVIDER_DIVIDEND (*(volatile u64*)0x04000290)
#define REG_DIV_DENOM (*(volatile u64*)0x04000298)
#define REG_DIV_RESULT (*(volatile s32*)0x040002a0)
#define IO_DIVIDER_QUOTIENT (*(volatile long long*)0x040002a0)
#define REG_DIVREM_RESULT (*(volatile s32*)0x040002a8)

/* DIVCNT mode 0: 32-bit numerator, 32-bit denominator. */
#define DIVCNT_MODE_32_32 0
/* DIVCNT mode 1: 64-bit numerator, 32-bit denominator (the denominator
 * register is still 64 bits wide). */
#define DIVCNT_MODE_64_32 1
/* DIVCNT mode 2: 64-bit numerator, 64-bit denominator. */
#define DIVCNT_MODE_64_64 2
/* DIVCNT bit 15: set while the divider is working. */
#define DIVCNT_BUSY 0x8000

#endif
