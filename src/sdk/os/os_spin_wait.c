#include "types.h"
#include "sdk/os.h"

/* Hand-written ARM assembly: busy-waits about cycles CPU cycles (4 per pass). */
asm void OS_SpinWait(u32 cycles)
{
loop:
    subs r0, r0, #4
    bcs loop
    bx lr
}
