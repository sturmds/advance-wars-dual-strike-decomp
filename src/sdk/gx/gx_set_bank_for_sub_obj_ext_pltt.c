#include "types.h"
#include "sdk/gx.h"
extern u16 data_022a9878[13];
void Vram_PowerOnBanks(u32 banks);
void func_0210a9e0(int bank)
{
    u32 lcdc = data_022a9878[0];
    u32 oldBank = data_022a9878[12];
    data_022a9878[12] = bank;
    data_022a9878[0] = (lcdc | oldBank) & ~bank;
    if (bank != 0) {
        if (bank == 0x100) {
            *REG_DB_DISPCNT |= 0x80000000;
            *REG_VRAMCNT_I = 0x83;
        }
    } else {
        *REG_DB_DISPCNT &= ~0x80000000;
    }
    Vram_PowerOnBanks(data_022a9878[0]);
}
