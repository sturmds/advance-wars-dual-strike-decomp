#include "sdk/wl.h"
#include "sdk/os_arm7.h"

void Wifi_ArmPeriodicMs(u32 milliseconds, OSAlarmHandler handler) {
  u32 period;
  OSTick start;

  Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->periodicAlarm);
  period = Tick_FromMilliseconds32(milliseconds);
  start = Tick_Read64_Arm7() + period;
  Alarm_ArmRepeating_Arm7(&WIFI_DRIVER_STATE->periodicAlarm, start, period,
                          handler, NULL);
}
