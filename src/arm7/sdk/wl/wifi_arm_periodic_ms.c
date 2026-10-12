#include "sdk/wl.h"

void Alarm_Withdraw_Arm7(OSAlarm *alarm);
void Alarm_ArmRepeating_Arm7(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg);

void Wifi_ArmPeriodicMs(u32 milliseconds, OSAlarmHandler handler)
{
    u32 period;
    OSTick start;

    Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->periodicAlarm);
    period = Tick_FromMilliseconds32(milliseconds);
    start = Tick_Read64_Arm7() + period;
    Alarm_ArmRepeating_Arm7(&WIFI_DRIVER_STATE->periodicAlarm, start, period, handler, NULL);
}
