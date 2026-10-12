#include "sdk/wl.h"

void Alarm_Withdraw_Arm7(OSAlarm *alarm);
void Alarm_ArmOneShot_Arm7(OSAlarm *alarm, OSTick delay, OSAlarmHandler handler, void *arg);

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler)
{
    Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm);
    Alarm_ArmOneShot_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm, Tick_FromMilliseconds32(milliseconds), handler, NULL);
}
