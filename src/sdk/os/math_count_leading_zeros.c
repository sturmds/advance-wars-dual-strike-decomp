#include "types.h"

/*
 * Hand-written ARM assembly: the clz instruction has no C form in this
 * compiler.
 */
asm u32 MATH_CountLeadingZeros(u32 value)
{
    clz r0, r0
    bx lr
}
