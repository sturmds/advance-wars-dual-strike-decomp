#include "types.h"
#include "sdk/gx.h"
extern u16 gGxVramBankMasks[13];
void func_0210a9e0(int bank)
{
    u32 lcdc = gGxVramBankMasks[0];
    u32 oldBank = gGxVramBankMasks[12];
    gGxVramBankMasks[12] = bank;
    gGxVramBankMasks[0] = (lcdc | oldBank) & ~bank;
    if (bank != 0) {
        if (bank == 0x100) {
            *REG_DB_DISPCNT |= 0x80000000;
            *REG_VRAMCNT_I = 0x83;
        }
    } else {
        *REG_DB_DISPCNT &= ~0x80000000;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
