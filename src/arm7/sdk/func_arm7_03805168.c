#include "sdk/pm.h"
#include "sdk/pxi.h"

void func_arm7_03805168(u32 data)
{
    while (Fifo_PushWord_Arm7(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
    }
}
