#ifndef SDK_GX_H
#define SDK_GX_H

#include "types.h"

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)
#define REG_BG2CNT ((volatile u16 *)0x0400000c)
#define REG_POWCNT1 ((volatile u16 *)0x04000304)
#define POWCNT1_DISPLAY_SWAP 0x8000 /* set: main engine on the top screen */

/* What GX_SetMasterBrightness calls: writes a master brightness register.
 * brightness: 0 none, 1-16 towards white, -1 to -16 towards black. */
void GXx_SetMasterBrightness_(volatile u16 *reg, int brightness);

#endif
