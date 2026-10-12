#include "types.h"

extern void func_arm7_037fb924(int lockID);

asm void func_arm7_037fb810(int lockID)
{
    ldr r1, =func_arm7_037fb924
    bx r1
}
