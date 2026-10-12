#include "sdk/os_arm7.h"
#include "sdk/snd.h"

void Alarm_ArmRepeating_Arm7(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg);
void SndDrv_OnIntervalAlarm(void *arg);

/* Ticks between driver updates: 2728 * 64 / 33.5 MHz, about 5.2 ms. */
#define SOUND_TICK_PERIOD 2728
/* Delay before the first update. */
#define SOUND_TICK_FIRST_DELAY 0x10000

void SND_StartIntervalTimer(void)
{
    OSTick now = Tick_Read64_Arm7();

    Alarm_ArmRepeating_Arm7(&data_arm7_03807bc4, now + SOUND_TICK_FIRST_DELAY, SOUND_TICK_PERIOD,
                             SndDrv_OnIntervalAlarm, NULL);
}
