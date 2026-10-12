#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 12 the
 * banks given to sub engine OBJ extended palettes. */
extern u16 gGxVramBankMasks[13];

#define DB_DISPCNT_OBJ_EXT_PLTT \
  0x80000000 /* sub engine OBJ extended palettes on */
#define VRAMCNT_SUB_OBJEXTPLTT \
  0x83 /* bank I: enabled, sub engine OBJ extended palette */

/* subObjExtPltt is bank I or no bank. With no bank, extended OBJ palettes are
 * switched off in the sub engine's DISPCNT. */
void GX_SetBankForSubOBJExtPltt(int subObjExtPltt) {
  gGxVramBankMasks[0] =
      ~subObjExtPltt & (gGxVramBankMasks[0] | gGxVramBankMasks[12]);
  gGxVramBankMasks[12] = subObjExtPltt;

  switch (subObjExtPltt) {
    case GX_VRAM_I:
      *REG_DB_DISPCNT |= DB_DISPCNT_OBJ_EXT_PLTT;
      *REG_VRAMCNT_I = VRAMCNT_SUB_OBJEXTPLTT;
      break;
    case 0:
      *REG_DB_DISPCNT &= ~DB_DISPCNT_OBJ_EXT_PLTT;
      break;
  }
  Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
