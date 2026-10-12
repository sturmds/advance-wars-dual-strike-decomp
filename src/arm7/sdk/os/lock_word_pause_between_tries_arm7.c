#include "types.h"
#include "sdk/os_arm7.h"

/* The pause between tries of LockWord_SpinAcquire_Arm7. */
void LockWord_PauseBetweenTries_Arm7(void)
{
    Cpu_BusyWaitCycles_Arm7(0x1000);
}
