#include "types.h"
#include "sdk/os_arm7.h"

void Irq_ResetWakeMaskAndFrames_Arm7(void)
{
    data_arm7_03807888 = 0;
    *SHARED_FRAME_COUNTER = 0;
}
