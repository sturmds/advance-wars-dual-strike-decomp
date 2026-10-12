#include "sdk/svc.h"

void Cpu_IdleUntilIrq_Arm7(void)
{
    Bios_HaltUntilIrq_Arm7();
}
