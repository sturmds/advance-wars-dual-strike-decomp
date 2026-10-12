#include "types.h"
#include "sdk/os_arm7.h"

void Alarm_Clear_Arm7(OSAlarm* alarm) {
  alarm->handler = NULL;
  alarm->tag = 0;
}
