#ifndef ROOT_UNIT_IO_H
#define ROOT_UNIT_IO_H

#include "types.h"

/* ARM9 hardware square root registers. */
#define IO_ROOT_UNIT_CTRL (*(volatile u16 *)0x040002b0)
#define REG_SQRT_RESULT (*(volatile u32 *)0x040002b4)
#define REG_SQRT_PARAM (*(volatile u32 *)0x040002b8)

/* SQRTCON mode 0: 32-bit parameter. */
#define SQRTCON_MODE_32 0
/* SQRTCON bit 15: set while the unit is working. */
#define SQRTCON_BUSY 0x8000

#endif
