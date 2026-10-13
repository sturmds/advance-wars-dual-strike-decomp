#include "types.h"

#ifdef __MWERKS__
/* Atomically stores value at *address and returns the old word. */
asm u32 Isd_SwapWord(u32 value, u32 *address)
{
    swp r0, r0, [r1]
    bx lr
}
#else
#error "Isd_SwapWord: an atomic swap of a word in one bus access (swp) has no portable C99 equivalent"
#endif
