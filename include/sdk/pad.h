#ifndef SDK_PAD_H
#define SDK_PAD_H

#include "types.h"

/* Only the ARM7 can read the X and Y buttons, the debug button and the lid
 * switch (EXTKEYIN). It copies them into a halfword in shared main RAM that
 * the ARM9 merges with its own KEYINPUT. */

/* RCNT: serial and general-purpose port mode, ARM7 only. */
#define REG_RCNT ((volatile u16 *)0x04000134)
#define RCNT_MODE_MASK 0xc000
#define RCNT_MODE_GPIO 0x8000
#define RCNT_SI_OUTPUT 0x0040          /* GPIO mode: SI pin direction */
#define RCNT_SI_IRQ 0x0100             /* GPIO mode: interrupt when SI falls */

/* EXTKEYIN, ARM7 only: a bit reads 0 while its button is down. */
#define REG_EXTKEYIN ((volatile u16 *)0x04000136)
#define EXTKEYIN_X     0x0001
#define EXTKEYIN_Y     0x0002
#define EXTKEYIN_DEBUG 0x0008
#define EXTKEYIN_FOLD  0x0080          /* set while the lid is closed */

/* The shared halfword: the EXTKEYIN button bits moved up to the KEYINPUT
 * positions of X (0x400), Y (0x800) and debug (0x2000), and bit 15 for a
 * closed lid. */
#define EXTRA_KEYS_MIRROR ((volatile u16 *)0x027fffa8)
#define EXTRA_KEYS_TO_KEYINPUT_SHIFT 10
#define EXTRA_KEYS_LID_CLOSED 0x8000

#endif
