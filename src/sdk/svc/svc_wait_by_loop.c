#include "sdk/svc.h"

/* BIOS call 0x03: busy-waits for count loop iterations. */
#pragma thumb on
asm void SVC_WaitByLoop(int count)
{
    swi 0x03
    bx lr
}
#pragma thumb reset
