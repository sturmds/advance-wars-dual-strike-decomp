#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x00: restarts the system. */
#pragma thumb on
asm void SVC_SoftReset(void)
{
    swi 0x00
    bx lr
}
#pragma thumb reset
#else
#error "SVC_SoftReset: restarting the system through the BIOS (call 0x00) has no portable C equivalent"
#endif
