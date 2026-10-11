#include "types.h"
#include "sdk/gx.h"
extern u16 gGxVramBankMasks[13];
void func_0210aa6c(int bank)
{
    u32 t = gGxVramBankMasks[11];
    gGxVramBankMasks[11] = bank;
    t = t | gGxVramBankMasks[0];
    gGxVramBankMasks[0] = t & ~bank;
    if (bank != 0) {
        if (bank == 0x80) {
            *REG_DB_DISPCNT |= 0x40000000;
            *REG_VRAMCNT_H = 0x82;
        }
    } else {
        *REG_DB_DISPCNT &= ~0x40000000;
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
