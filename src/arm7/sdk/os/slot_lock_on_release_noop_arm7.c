#include "types.h"
#include "sdk/os_arm7.h"

/* Callback of the cartridge lock word. On the ARM9 it hands the bus over; the ARM7 has nothing to do. */
void SlotLock_OnReleaseNoop_Arm7(void) {}
