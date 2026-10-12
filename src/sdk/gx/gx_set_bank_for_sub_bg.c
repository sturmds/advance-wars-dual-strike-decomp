#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 9 the
 * banks given to sub engine BG data. */
extern u16 gGxVramBankMasks[];

/* VRAMCNT values for sub engine BG (GBATEK): bank C uses mode 4, H and I mode 1. */
#define VRAMCNT_SUB_BG_C 0x84
#define VRAMCNT_SUB_BG_HI 0x81

/* sub_bg is C, H, or H and I together; H and I are mapped one after the other. */
void GX_SetBankForSubBG(int sub_bg) {
  gGxVramBankMasks[0] = ~sub_bg & (gGxVramBankMasks[0] | gGxVramBankMasks[9]);
  gGxVramBankMasks[9] = sub_bg;

  switch (sub_bg) {
    case 0:
      break;
    case GX_VRAM_C:
      *REG_VRAMCNT_C = VRAMCNT_SUB_BG_C;
      break;
    case GX_VRAM_H | GX_VRAM_I:
      *REG_VRAMCNT_I = VRAMCNT_SUB_BG_HI;
    case GX_VRAM_H:
      *REG_VRAMCNT_H = VRAMCNT_SUB_BG_HI;
      break;
  }
  Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
