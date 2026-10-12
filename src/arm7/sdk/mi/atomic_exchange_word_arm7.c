#include "sdk/os_arm7.h"

asm u32 Atomic_ExchangeWord_Arm7(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
