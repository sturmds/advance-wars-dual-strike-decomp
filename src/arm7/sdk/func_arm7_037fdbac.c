#include "types.h"
#include "sdk/os_arm7.h"

asm OSIntrMode func_arm7_037fdbac(void)
{
    mrs r0, CPSR
    orr r1, r0, #0xc0
    msr CPSR_c, r1
    and r0, r0, #0xc0
    bx lr
}
