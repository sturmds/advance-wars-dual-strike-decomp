#include "types.h"

extern u16 data_arm7_03807ab0;

u16 Reset_WasRequested_Arm7(void)
{
    return data_arm7_03807ab0;
}
