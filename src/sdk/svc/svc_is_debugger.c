#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x0f: reports whether a debugger is attached. */
#pragma thumb on
asm int SVC_IsDebugger(void)
{
    swi 0x0f
    bx lr
}
#pragma thumb reset
#else
#error "SVC_IsDebugger: asking the BIOS whether debugging hardware is present (call 0x0f) has no portable C equivalent"
#endif
