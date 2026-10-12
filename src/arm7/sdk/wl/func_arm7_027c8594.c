#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

void func_arm7_027c8594(void) {
  Alarm_Withdraw_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm);
}
