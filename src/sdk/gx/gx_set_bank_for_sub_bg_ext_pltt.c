#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 11 the
 * bank given to sub engine BG extended palettes. */

/* subBgExtPltt is H or none. With no bank, extended BG palettes are switched
 * off in the sub engine's DISPCNT. */
void GX_SetBankForSubBGExtPltt(int subBgExtPltt) {
  gGxVramBankMasks[0] =
      ~subBgExtPltt & (gGxVramBankMasks[0] | gGxVramBankMasks[11]);
  gGxVramBankMasks[11] = subBgExtPltt;

  switch (subBgExtPltt) {
    case GX_VRAM_H:
      *REG_DB_DISPCNT |= DISPCNT_BG_EXT_PLTT;
      *REG_VRAMCNT_H = VRAMCNT_SUB_BGEXTPLTT;
      break;
    case 0:
      *REG_DB_DISPCNT &= ~DISPCNT_BG_EXT_PLTT;
      break;
  }
  Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
