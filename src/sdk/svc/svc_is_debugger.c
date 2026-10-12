#include "sdk/svc.h"

/* BIOS call 0x0f: reports whether a debugger is attached. */
#pragma thumb on
asm int SVC_IsDebugger(void)
{
    swi 0x0f
    bx lr
}
#pragma thumb reset
