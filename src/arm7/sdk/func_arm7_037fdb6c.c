#include "types.h"

asm int func_arm7_037fdb6c(void)
{
    mrs r0, CPSR
    bic r1, r0, #0x80
    msr CPSR_c, r1
    and r0, r0, #0x80
    bx lr
}
