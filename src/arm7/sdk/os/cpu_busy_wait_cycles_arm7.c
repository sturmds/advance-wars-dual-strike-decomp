#include "types.h"
#include "sdk/os_arm7.h"

/* Busy-waits about cycles CPU cycles; the BIOS wait loop takes 4 cycles a turn. */
void Cpu_BusyWaitCycles_Arm7(s32 cycles) {
  Cpu_DelayLoop_Arm7(_s32_div_f(cycles, 4));
}
