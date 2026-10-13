#include "types.h"

extern void OS_UnlockCartridge(int lockID);

#ifdef __MWERKS__
/*
 * Tail call to OS_UnlockCartridge with the caller's argument. The compiler
 * always picks r12 for the jump register, the original uses r1, so this
 * is written as assembly.
 */
asm void func_0210cee4(int lockID)
{
    ldr r1, =OS_UnlockCartridge
    bx r1
}
#else
void func_0210cee4(int lockID) { OS_UnlockCartridge(lockID); }
#endif
