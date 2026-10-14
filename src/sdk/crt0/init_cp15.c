#include "types.h"

extern char DTCM_BASE[]; /* start of DTCM (ARM9_LAYOUT_SYMBOLS) */

#ifdef __MWERKS__
/*
 * Sets up the ARM946E-S system control coprocessor (CP15) at boot. Turns
 * off the protection unit, caches and TCMs, invalidates both caches and
 * drains the write buffer, then programs the eight protection regions
 * (each word holds base | size code << 1 | enable):
 *   0 I/O registers 64 MB, 1 main RAM 8 MB, 2 top 256 KB of main RAM,
 *   3 GBA slot 128 MB, 4 DTCM 16 KB, 5 ITCM 16 MB, 6 BIOS 32 KB,
 *   7 shared main RAM 4 KB.
 * Region 2's word 0x027c0023 (base 0x027c0000, size code 0x11, enable)
 * is a plain constant, not an address in DTCM.
 * Next come the ITCM and DTCM base registers, the cacheable bits (regions
 * 1 and 6), the write-buffer bit (region 1), the data and instruction
 * access permissions, and last the control register bits that turn it all
 * on.
 */
asm void init_cp15(void)
{
    mrc p15, 0, r0, c1, c0, 0
    ldr r1, =0x000f9005
    bic r0, r0, r1
    mcr p15, 0, r0, c1, c0, 0
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0
    mcr p15, 0, r0, c7, c6, 0
    mcr p15, 0, r0, c7, c10, 4
    ldr r0, =0x04000033
    mcr p15, 0, r0, c6, c0, 0
    ldr r0, =0x0200002d
    mcr p15, 0, r0, c6, c1, 0
    ldr r0, =0x027c0023
    mcr p15, 0, r0, c6, c2, 0
    ldr r0, =0x08000035
    mcr p15, 0, r0, c6, c3, 0
    ldr r0, =DTCM_BASE
    orr r0, r0, #0x1a
    orr r0, r0, #1
    mcr p15, 0, r0, c6, c4, 0
    ldr r0, =0x0100002f
    mcr p15, 0, r0, c6, c5, 0
    ldr r0, =0xffff001d
    mcr p15, 0, r0, c6, c6, 0
    ldr r0, =0x027ff017
    mcr p15, 0, r0, c6, c7, 0
    mov r0, #0x20
    mcr p15, 0, r0, c9, c1, 1
    ldr r0, =DTCM_BASE
    orr r0, r0, #0xa
    mcr p15, 0, r0, c9, c1, 0
    mov r0, #0x42
    mcr p15, 0, r0, c2, c0, 1
    mov r0, #0x42
    mcr p15, 0, r0, c2, c0, 0
    mov r0, #2
    mcr p15, 0, r0, c3, c0, 0
    ldr r0, =0x05100011
    mcr p15, 0, r0, c5, c0, 3
    ldr r0, =0x15111011
    mcr p15, 0, r0, c5, c0, 2
    mrc p15, 0, r0, c1, c0, 0
    ldr r1, =0x0005707d
    orr r0, r0, r1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}
#else
#error "init_cp15: programming the CP15 protection unit, caches and TCMs (mcr/mrc) has no portable C equivalent"
#endif
