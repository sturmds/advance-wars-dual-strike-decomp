#ifndef SDK_GX_H
#define SDK_GX_H

#include "types.h"

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)

/* What GX_SetMasterBrightness calls: writes a master brightness register.
 * brightness: 0 none, 1-16 towards white, -1 to -16 towards black. */
void GXx_SetMasterBrightness_(volatile u16 *reg, int brightness);

#endif
