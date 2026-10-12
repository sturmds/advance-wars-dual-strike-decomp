#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"
#include "sdk/ctrdg.h"

void System_StartServices_Arm7(void)
{
    Arena_ApplyDefaults_Arm7();
    Fifo_SetUp_Arm7();
    LockId_HandshakeAndFreeIds_Arm7();
    Irq_ResetWakeMaskAndFrames_Arm7();
    Tick_StartTimer0_Arm7();
    Alarm_SetUpTimer1Queue_Arm7();
    Thread_SetUpTableOnce_Arm7();
    Reset_ListenForRequest_Arm7();
    Cartridge_SetUpOnce_Arm7();
}
