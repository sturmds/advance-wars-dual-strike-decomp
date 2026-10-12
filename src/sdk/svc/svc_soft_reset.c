#include "sdk/svc.h"

/* BIOS call 0x00: restarts the system. */
#pragma thumb on
asm void SVC_SoftReset(void)
{
    swi 0x00
    bx lr
}
#pragma thumb reset
