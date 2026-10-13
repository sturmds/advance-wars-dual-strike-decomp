#include "types.h"
#include "sdk/os.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly: busy-waits about cycles CPU cycles (4 per pass). */
asm void OS_SpinWait(u32 cycles)
{
loop:
    subs r0, r0, #4
    bcs loop
    bx lr
}
#else
#error "OS_SpinWait: waiting a number of CPU cycles set by the timing of the assembly loop has no portable C equivalent"
#endif
