#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/snd.h"

void SND_StopIntervalTimer(void)
{
    Alarm_Withdraw_Arm7(&data_arm7_03807bc4);
}
