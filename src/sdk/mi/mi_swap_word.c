#include "sdk/os.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly: swp stores setData at destp and returns the
 * old value in one atomic step. It has no C form.
 */
asm u32 MI_SwapWord(u32 setData, volatile u32 *destp)
{
    swp r0, r0, [r1]
    bx lr
}
#else
#error "MI_SwapWord: swapping a word with memory in one atomic step (swp) has no portable C equivalent"
#endif
