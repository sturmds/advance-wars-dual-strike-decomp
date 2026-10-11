#include "types.h"
#include "sdk/gx.h"
extern u16 data_022a9878[13];
void Vram_PowerOnBanks(u32 banks);
void func_0210aa6c(int bank)
{
    u32 t = data_022a9878[11];
    data_022a9878[11] = bank;
    t = t | data_022a9878[0];
    data_022a9878[0] = t & ~bank;
    if (bank != 0) {
        if (bank == 0x80) {
            *REG_DB_DISPCNT |= 0x40000000;
            *REG_VRAMCNT_H = 0x82;
        }
    } else {
        *REG_DB_DISPCNT &= ~0x40000000;
    }
    Vram_PowerOnBanks(data_022a9878[0]);
}
