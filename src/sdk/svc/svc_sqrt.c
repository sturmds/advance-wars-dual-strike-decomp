#include "sdk/svc.h"

/* BIOS call 0x0d: integer square root. */
#pragma thumb on
asm u32 SVC_Sqrt(u32 value)
{
    swi 0x0d
    bx lr
}
#pragma thumb reset
