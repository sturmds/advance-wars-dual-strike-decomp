#include "types.h"

asm u32 func_arm7_037fdbd8(void)
{
    mrs r0, CPSR
    and r0, r0, #0x1f
    bx lr
}
