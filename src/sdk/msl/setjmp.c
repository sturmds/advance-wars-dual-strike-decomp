#include "types.h"

/*
 * Saves r4-r11, sp and lr into buf (ten words) and returns 0. longjmp
 * returns here a second time with a nonzero value.
 */
asm int setjmp(void *buf)
{
    stmia r0, {r4-r11, sp, lr}
    mov r0, #0
    bx lr
}
