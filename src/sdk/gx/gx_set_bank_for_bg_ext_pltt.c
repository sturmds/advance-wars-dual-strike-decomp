#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 7 the
 * banks given to main engine BG extended palettes. */
extern u16 gGxVramBankMasks[];

/* bgExtPltt is E (all four slots), F (slots 0-1), G (slots 2-3) or F and G.
 * With no banks, extended BG palettes are switched off in DISPCNT. */
void GX_SetBankForBGExtPltt(int bgExtPltt)
{
    gGxVramBankMasks[0] = ~bgExtPltt & (gGxVramBankMasks[0] | gGxVramBankMasks[7]);
    gGxVramBankMasks[7] = bgExtPltt;

    switch (bgExtPltt) {
    case GX_VRAM_E:
        *REG_DISPCNT |= DISPCNT_BG_EXT_PLTT;
        *REG_VRAMCNT_E = VRAMCNT_BGEXTPLTT(0);
        break;
    case GX_VRAM_G:
        *REG_DISPCNT |= DISPCNT_BG_EXT_PLTT;
        *REG_VRAMCNT_G = VRAMCNT_BGEXTPLTT(1);
        break;
    case GX_VRAM_F | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_BGEXTPLTT(1);
    case GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_BGEXTPLTT(0);
        *REG_DISPCNT |= DISPCNT_BG_EXT_PLTT;
        break;
    case 0:
        *REG_DISPCNT &= ~DISPCNT_BG_EXT_PLTT;
        break;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
