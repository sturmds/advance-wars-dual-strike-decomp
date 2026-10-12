#include "sdk/wl.h"
#include "sdk/os_arm7.h"

void Wifi_CreateAlarms(void) {
  WlWork* work = WIFI_DRIVER_STATE;

  if (!Alarm_IsSetUp_Arm7()) {
    work->state.unk_12 |= 0x40;
  } else {
    Alarm_Clear_Arm7(&work->periodicAlarm);
    Alarm_Clear_Arm7(&work->oneShotAlarm);
  }
}
