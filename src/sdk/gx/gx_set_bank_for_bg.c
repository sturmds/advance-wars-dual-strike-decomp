#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 1 the
 * banks given to main engine BG data. */
extern u16 gGxVramBankMasks[];

/* bg is any mix of A-D, or any mix of E-G; the banks are mapped one after
 * another in BG VRAM, lowest bank first. */
void GX_SetBankForBG(int bg)
{
    gGxVramBankMasks[0] = ~bg & (gGxVramBankMasks[0] | gGxVramBankMasks[1]);
    gGxVramBankMasks[1] = bg;

    switch (bg) {
    case GX_VRAM_D:
        *REG_VRAMCNT_D = VRAMCNT_BG(0);
        break;
    case GX_VRAM_C | GX_VRAM_D:
        *REG_VRAMCNT_D = VRAMCNT_BG(1);
    case GX_VRAM_C:
        *REG_VRAMCNT_C = VRAMCNT_BG(0);
        break;
    case GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
        *REG_VRAMCNT_D = VRAMCNT_BG(2);
    case GX_VRAM_B | GX_VRAM_C:
        *REG_VRAMCNT_C = VRAMCNT_BG(1);
    case GX_VRAM_B:
        *REG_VRAMCNT_B = VRAMCNT_BG(0);
        break;
    case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
        *REG_VRAMCNT_D = VRAMCNT_BG(3);
    case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C:
        *REG_VRAMCNT_C = VRAMCNT_BG(2);
    case GX_VRAM_A | GX_VRAM_B:
        *REG_VRAMCNT_B = VRAMCNT_BG(1);
    case GX_VRAM_A:
        *REG_VRAMCNT_A = VRAMCNT_BG(0);
        break;
    case GX_VRAM_A | GX_VRAM_B | GX_VRAM_D:
        *REG_VRAMCNT_A = VRAMCNT_BG(0);
        *REG_VRAMCNT_B = VRAMCNT_BG(1);
        *REG_VRAMCNT_D = VRAMCNT_BG(2);
        break;
    case GX_VRAM_A | GX_VRAM_C | GX_VRAM_D:
        *REG_VRAMCNT_D = VRAMCNT_BG(2);
    case GX_VRAM_A | GX_VRAM_C:
        *REG_VRAMCNT_A = VRAMCNT_BG(0);
        *REG_VRAMCNT_C = VRAMCNT_BG(1);
        break;
    case GX_VRAM_A | GX_VRAM_D:
        *REG_VRAMCNT_A = VRAMCNT_BG(0);
        *REG_VRAMCNT_D = VRAMCNT_BG(1);
        break;
    case GX_VRAM_B | GX_VRAM_D:
        *REG_VRAMCNT_B = VRAMCNT_BG(0);
        *REG_VRAMCNT_D = VRAMCNT_BG(1);
        break;
    case GX_VRAM_E | GX_VRAM_F | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_BG(3);
    case GX_VRAM_E | GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_BG(2);
    case GX_VRAM_E:
        *REG_VRAMCNT_E = VRAMCNT_BG(0);
        break;
    case GX_VRAM_E | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_BG(2);
        *REG_VRAMCNT_E = VRAMCNT_BG(0);
        break;
    case GX_VRAM_F | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_BG(1);
    case GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_BG(0);
        break;
    case GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_BG(0);
        break;
    case 0:
        break;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
