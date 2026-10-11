#include "types.h"

extern void func_0210d064(int lockID);

/*
 * Tail call to func_0210d064 with the caller's argument. The compiler
 * always picks r12 for the jump register, the original uses r1, so this
 * is written as assembly.
 */
asm void func_0210cee4(int lockID)
{
    ldr r1, =func_0210d064
    bx r1
}
