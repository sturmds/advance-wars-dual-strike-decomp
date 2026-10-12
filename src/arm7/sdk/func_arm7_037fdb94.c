#include "types.h"
#include "sdk/os_arm7.h"

asm void func_arm7_037fdb94(int state)
{
    mrs r1, CPSR
    bic r2, r1, #0x80
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #0x80
    bx lr
}
