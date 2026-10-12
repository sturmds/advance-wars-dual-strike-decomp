#include "types.h"
#include "sdk/gx_load.h"

extern int data_022a9894;
void GX_SetBankForSubBGExtPltt(int bank);

void GXS_EndLoadBGExtPltt(void)
{
    if (data_0216d958 != -1) {
        MI_WaitDma(data_0216d958);
    }
    GX_SetBankForSubBGExtPltt(data_022a9894);
    data_022a9894 = 0;
}
