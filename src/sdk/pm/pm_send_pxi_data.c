#include "sdk/pm.h"

int PXI_SendWordByFifo(int tag, u32 data, BOOL err);

void PMi_SendPxiData(u32 data)
{
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
    }
}
