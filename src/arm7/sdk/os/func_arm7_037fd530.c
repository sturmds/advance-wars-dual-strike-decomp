#include "types.h"
#include "sdk/os_arm7.h"

extern s32 data_arm7_03807a9c;
extern s32 data_arm7_03807aa0;

/*
 * V-count alarm frame counter: a V-count below the one seen last means a
 * new frame started. Records vcount and returns the frame count.
 */
s32 func_arm7_037fd530(s32 vcount)
{
    int irqState = Cpu_MaskIrq_Arm7();

    if (vcount < data_arm7_03807a9c) {
        data_arm7_03807aa0++;
    }
    data_arm7_03807a9c = vcount;
    Cpu_RestoreIrqBit_Arm7(irqState);
    return data_arm7_03807aa0;
}
