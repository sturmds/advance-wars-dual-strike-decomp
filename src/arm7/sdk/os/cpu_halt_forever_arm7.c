#include "types.h"
#include "sdk/os_arm7.h"

void Cpu_HaltForever_Arm7(void)
{
    while (TRUE) {
        Cpu_MaskIrq_Arm7();
        Cpu_IdleUntilIrq_Arm7();
    }
}
