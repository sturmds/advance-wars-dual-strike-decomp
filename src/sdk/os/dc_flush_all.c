#include "types.h"

/*
 * DC_FlushAll: cleans and invalidates the whole data cache by set and way.
 * The loop steps the set index (bits 5 to 9) through 32 sets for each of
 * the four ways (bits 30 and 31).
 */
asm void DC_FlushAll(void)
{
    mov r1, #0
way_loop:
    mov r0, #0
set_loop:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c14, 2
    add r0, r0, #0x20
    cmp r0, #0x400
    blt set_loop
    add r1, r1, #0x40000000
    cmp r1, #0
    bne way_loop
    bx lr
}
