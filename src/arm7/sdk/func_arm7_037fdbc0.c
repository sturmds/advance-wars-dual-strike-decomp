#include "types.h"
#include "sdk/os_arm7.h"

asm OSIntrMode func_arm7_037fdbc0(OSIntrMode state)
{
    mrs r1, CPSR
    bic r2, r1, #0xc0
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #0xc0
    bx lr
}
