#include "sdk/os_arm7.h"

#ifdef __MWERKS__
/* Swaps setData with the word at destp in one atomic step (swp) and
   returns the old word. */
asm u32 Atomic_ExchangeWord_Arm7(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
#else
#error "Atomic_ExchangeWord_Arm7: an atomic swap of a register with memory (swp) has no portable C equivalent"
#endif
