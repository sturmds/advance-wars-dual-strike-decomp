#include "types.h"
#include "sdk/os.h"

asm int func_arm7_037fdb80(void)
{
    mrs r0, CPSR
    orr r1, r0, #0x80
    msr CPSR_c, r1
    and r0, r0, #0x80
    bx lr
}
