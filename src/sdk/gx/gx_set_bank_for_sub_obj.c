#include "types.h"
#include "sdk/gx.h"

extern u16 data_022a9878[13];
void Vram_PowerOnBanks(u32 banks);

void func_0210aaf8(int bank)
{
    u32 oldBank;
    u32 lcdc;
    lcdc = data_022a9878[0];
    oldBank = data_022a9878[10];

    data_022a9878[10] = bank;
    data_022a9878[0] = (oldBank | lcdc) & ~bank;
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
    Vram_PowerOnBanks(data_022a9878[0]);
}
