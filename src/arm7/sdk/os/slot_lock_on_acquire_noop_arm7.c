#include "types.h"
#include "sdk/os_arm7.h"

/* Callback of the cartridge lock word. On the ARM9 it takes the bus; the ARM7 has nothing to do. */
void SlotLock_OnAcquireNoop_Arm7(void) {}
