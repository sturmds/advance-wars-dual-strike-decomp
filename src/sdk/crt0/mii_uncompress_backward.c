#include "types.h"

/*
 * Unpacks a module compressed from its end, in place. The two words before
 * `bottom` hold the compressed length (low 24 bits) with the footer length
 * in the top byte, and how many bytes the module grows by. Reads go down
 * from below the footer and writes go down from bottom + growth: a flag
 * byte covers eight items, MSB first; a clear bit copies one byte, a set
 * bit copies (top nibble + 3) bytes from (low 12 bits + 3) above the write
 * position. Then cleans the data cache and invalidates the instruction
 * cache over the unpacked range, 32-byte lines. Does nothing for NULL.
 */
asm void MIi_UncompressBackward(void *bottom)
{
    cmp r0, #0
    beq done
    stmfd sp!, {r4, r5, r6, r7}
    ldmdb r0, {r1, r2}
    add r2, r0, r2
    sub r3, r0, r1, lsr #24
    bic r1, r1, #0xff000000
    sub r1, r0, r1
    mov r4, r2
next_flags:
    cmp r3, r1
    ble flush
    ldrb r5, [r3, #-1]!
    mov r6, #8
next_item:
    subs r6, r6, #1
    blt next_flags
    tst r5, #0x80
    bne copy_back
    ldrb r0, [r3, #-1]!
    strb r0, [r2, #-1]!
    b item_done
copy_back:
    ldrb r12, [r3, #-1]!
    ldrb r7, [r3, #-1]!
    orr r7, r7, r12, lsl #8
    bic r7, r7, #0xf000
    add r7, r7, #2
    add r12, r12, #0x20
copy_byte:
    ldrb r0, [r2, r7]
    strb r0, [r2, #-1]!
    subs r12, r12, #0x10
    bge copy_byte
item_done:
    cmp r3, r1
    mov r5, r5, lsl #1
    bgt next_item
flush:
    bic r3, r1, #31
flush_line:
    mcr p15, 0, r3, c7, c5, 1
    mcr p15, 0, r3, c7, c14, 1
    add r3, r3, #32
    cmp r3, r4
    blt flush_line
    ldmfd sp!, {r4, r5, r6, r7}
done:
    bx lr
}
