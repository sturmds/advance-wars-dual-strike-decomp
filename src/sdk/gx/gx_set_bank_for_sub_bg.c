#include "types.h"
#include "sdk/gx.h"

extern u16 gGxVramBankMasks[13];

void func_0210ab74(int bank)
{
    u32 oldBank = gGxVramBankMasks[9];
    u32 lcdc = gGxVramBankMasks[0];

    gGxVramBankMasks[9] = bank;
    gGxVramBankMasks[0] = (oldBank | lcdc) & ~bank;
    switch (bank) {
    case 0:
        break;
    case 4:
        *REG_VRAMCNT_C = 0x84;
        break;
    case 0x180:
        *REG_VRAMCNT_I = 0x81;
        /* fall through */
    case 0x80:
        *REG_VRAMCNT_H = 0x81;
        break;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
