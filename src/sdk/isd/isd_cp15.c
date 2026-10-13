#include "types.h"

#ifdef __MWERKS__
/*
 * Reads and writes of CP15 registers: protection region 7, data access permissions
 * and instruction access permissions. func_02121b80 and func_02121b88 are reads of
 * cache operation registers (c7, c10 and c6) with no caller; their job is unclear.
 */

asm u32 func_02121b80(void)
{
    mrc p15, 0, r0, c7, c10, 1
    bx lr
}

asm u32 func_02121b88(void)
{
    mrc p15, 0, r0, c7, c6, 1
    bx lr
}

asm u32 Isd_GetProtectionRegion7(void)
{
    mrc p15, 0, r0, c6, c7, 0
    bx lr
}

asm void Isd_SetProtectionRegion7(u32 value)
{
    mcr p15, 0, r0, c6, c7, 0
    bx lr
}

asm u32 Isd_GetDataPermissions(void)
{
    mrc p15, 0, r0, c5, c0, 0
    bx lr
}

asm void Isd_SetDataPermissions(u32 value)
{
    mcr p15, 0, r0, c5, c0, 0
    bx lr
}

asm u32 Isd_GetInstructionPermissions(void)
{
    mrc p15, 0, r0, c5, c0, 1
    bx lr
}

asm void Isd_SetInstructionPermissions(u32 value)
{
    mcr p15, 0, r0, c5, c0, 1
    bx lr
}
#else
#error "Isd_GetProtectionRegion7 and the other CP15 accessors: reading and writing CP15 registers (mrc, mcr) has no portable C equivalent"
#endif
