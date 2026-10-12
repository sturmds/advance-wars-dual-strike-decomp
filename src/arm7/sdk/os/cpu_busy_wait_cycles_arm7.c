#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037fdc04(s32 count);

/* Busy-waits about cycles CPU cycles; the BIOS wait loop takes 4 cycles a turn. */
void Cpu_BusyWaitCycles_Arm7(s32 cycles)
{
    func_arm7_037fdc04((s32)func_arm7_03806d4c(cycles, 4));
}
