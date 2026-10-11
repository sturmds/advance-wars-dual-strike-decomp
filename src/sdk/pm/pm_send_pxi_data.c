#include "sdk/pm.h"

int func_02110f88(int tag, u32 data, BOOL err);

void PMi_SendPxiData(u32 data)
{
    while (func_02110f88(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
    }
}
