#include "types.h"
#include "sdk/gx.h"

/* The 13 VRAM-bank masks start at data_022a9878; other functions still use it as a lone u16. */
extern u16 data_022a9878[13];

/* Clears the bank masks and the control register of every VRAM bank. */
void Vram_ClearAllBankControl(void)
{
    data_022a9878[0] = 0;
    data_022a9878[1] = 0;
    data_022a9878[2] = 0;
    data_022a9878[3] = 0;
    data_022a9878[4] = 0;
    data_022a9878[5] = 0;
    data_022a9878[6] = 0;
    data_022a9878[7] = 0;
    data_022a9878[8] = 0;
    data_022a9878[9] = 0;
    data_022a9878[10] = 0;
    data_022a9878[11] = 0;
    data_022a9878[12] = 0;
    *REG_VRAMCNT_ABCD = 0;
    *REG_VRAMCNT_E = 0;
    *REG_VRAMCNT_F = 0;
    *REG_VRAMCNT_G = 0;
    *IO_VRAM_BANKS_H_I_CTRL = 0;
}
