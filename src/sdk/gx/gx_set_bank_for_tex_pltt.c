#include "types.h"
#include "sdk/gx.h"

/* Bank assignment: index 0 holds the banks not given to any use, index 5 the
 * banks given to texture palettes. */
extern u16 data_022a9878[];

/* texPltt is a mask of banks E, F and G; each bank in it gets the next
 * texture palette slot, counting from the lowest bank. */
void GX_SetBankForTexPltt(int texPltt)
{
    data_022a9878[0] = ~texPltt & (data_022a9878[0] | data_022a9878[5]);
    data_022a9878[5] = texPltt;

    switch (texPltt) {
    case 0:
        break;
    case GX_VRAM_G:
        *REG_VRAMCNT_G = VRAMCNT_TEXPLTT(0);
        break;
    case GX_VRAM_G | GX_VRAM_F:
        *REG_VRAMCNT_G = VRAMCNT_TEXPLTT(1);
    case GX_VRAM_F:
        *REG_VRAMCNT_F = VRAMCNT_TEXPLTT(0);
        break;
    case GX_VRAM_G | GX_VRAM_F | GX_VRAM_E:
        *REG_VRAMCNT_G = VRAMCNT_TEXPLTT(3);
    case GX_VRAM_F | GX_VRAM_E:
        *REG_VRAMCNT_F = VRAMCNT_TEXPLTT(2);
    case GX_VRAM_E:
        *REG_VRAMCNT_E = VRAMCNT_TEXPLTT(0);
        break;
    }
    Vram_PowerOnBanks(data_022a9878[0]);
}
