#include "sdk/os.h"

/*
 * Hand-written ARM assembly: swp stores setData at destp and returns the
 * old value in one atomic step. It has no C form.
 */
asm u32 MI_SwapWord(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
