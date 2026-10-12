#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

void Arena_ApplyDefaults_Arm7(void);
void Tick_StartTimer0_Arm7(void);
void Alarm_SetUpTimer1Queue_Arm7(void);
void func_arm7_03806ae8(void);

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
    func_arm7_03806ae8();
}
