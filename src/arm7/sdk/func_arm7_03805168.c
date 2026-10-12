#include "sdk/pm.h"

int func_arm7_037fe1cc(int tag, u32 data, BOOL err);

void func_arm7_03805168(u32 data)
{
    while (func_arm7_037fe1cc(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
    }
}
