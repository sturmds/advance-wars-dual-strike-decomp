#include "sdk/wl.h"

void Alarm_Withdraw_Arm7(OSAlarm *alarm);
void Alarm_ArmOneShot_Arm7(OSAlarm *alarm, OSTick delay, OSAlarmHandler handler, void *arg);

void Wifi_ArmOneShotUs(u32 microseconds, OSAlarmHandler handler)
{
    Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm);
    Alarm_ArmOneShot_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm, Tick_FromMicroseconds32(microseconds), handler, NULL);
}
