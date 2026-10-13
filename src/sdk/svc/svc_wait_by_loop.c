#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x03: busy-waits for count loop iterations. */
#pragma thumb on
asm void SVC_WaitByLoop(int count)
{
    swi 0x03
    bx lr
}
#pragma thumb reset
#else
#error "SVC_WaitByLoop: a BIOS busy-wait timed in CPU cycles (call 0x03) has no portable C equivalent"
#endif
