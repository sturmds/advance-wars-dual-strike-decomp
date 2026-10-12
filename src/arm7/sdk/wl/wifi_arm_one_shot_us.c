#include "sdk/wl.h"
#include "sdk/os_arm7.h"

void Wifi_ArmOneShotUs(u32 microseconds, OSAlarmHandler handler)
{
    Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm);
    Alarm_ArmOneShot_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm, Tick_FromMicroseconds32(microseconds), handler, NULL);
}
