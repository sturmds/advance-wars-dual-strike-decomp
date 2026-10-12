#include "types.h"
#include "sdk/gx.h"

extern u16 gGxVramBankMasks[13];

void func_0210aaf8(int bank)
{
    gGxVramBankMasks[0] = ~bank & (gGxVramBankMasks[0] | gGxVramBankMasks[10]);
    gGxVramBankMasks[10] = bank;
    if (bank != 0) {
        switch (bank) {
        case 8:
            *REG_VRAMCNT_D = 0x84;
            break;
        case 0x100:
            *REG_VRAMCNT_I = 0x82;
            break;
        }
    }
    Vram_PowerOnBanks(gGxVramBankMasks[0]);
}
