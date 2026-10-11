#include "types.h"

/* Atomically stores value at *address and returns the old word. */
asm u32 Isd_SwapWord(u32 value, u32 *address)
{
    swp r0, r0, [r1]
    bx lr
}
