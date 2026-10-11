#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 2 the
 * banks given to main engine OBJ data. */
extern u16 gGxVramBankMasks[];

/* obj is A, B or both, or any mix of E, F and G; the banks are mapped one
 * after another in OBJ VRAM, lowest bank first. */
void GX_SetBankForOBJ(int obj)
{
    gGxVramBankMasks[0] = ~obj & (gGxVramBankMasks[0] | gGxVramBankMasks[2]);
    gGxVramBankMasks[2] = obj;

    switch (obj) {
    case GX_VRAM_A | GX_VRAM_B:
        *REG_VRAMCNT_B = VRAMCNT_OBJ(1);
    case GX_VRAM_A:
        *REG_VRAMCNT_A = VRAMCNT_OBJ(0);
        break;
    case GX_VRAM_B:
        *REG_VRAMCNT_B = VRAMCNT_OBJ(0);
        break;
    case GX_VRAM_E | GX_VRAM_F | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_OBJ(3);
    case GX_VRAM_E | GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_OBJ(2);
    case GX_VRAM_E:
        *REG_VRAMCNT_E = VRAMCNT_OBJ(0);
        break;
    case GX_VRAM_E | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_OBJ(2);
        *REG_VRAMCNT_E = VRAMCNT_OBJ(0);
        break;
    case GX_VRAM_F | GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_OBJ(1);
    case GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_OBJ(0);
        break;
    case GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_OBJ(0);
        break;
    case 0:
        break;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
