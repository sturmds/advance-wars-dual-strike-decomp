#include "types.h"
#include "sdk/gx.h"

extern u16 data_022a9878[13];
void Vram_PowerOnBanks(u32 banks);

void func_0210ab74(int bank)
{
    u32 oldBank = data_022a9878[9];
    u32 lcdc = data_022a9878[0];

    data_022a9878[9] = bank;
    data_022a9878[0] = (oldBank | lcdc) & ~bank;
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
    Vram_PowerOnBanks(data_022a9878[0]);
}
