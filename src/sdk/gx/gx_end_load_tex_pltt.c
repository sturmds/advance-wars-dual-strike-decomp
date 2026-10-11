#include "types.h"
#include "sdk/gx_load.h"

void GX_SetBankForTexPltt(int bank);

void GX_EndLoadTexPltt(void)
{
    if (data_0216d958 != -1) {
        MI_WaitDma(data_0216d958);
    }
    GX_SetBankForTexPltt(data_022a98ac);
    data_022a98ac = 0;
    data_022a98a8 = 0;
}
