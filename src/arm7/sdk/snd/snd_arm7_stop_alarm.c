#include "sdk/snd.h"
#include "sdk/os_arm7.h"

void SndArm7_StopAlarm(int alarmNo) {
  SndArm7Alarm* alarm = &data_arm7_03809010[alarmNo];

  if (alarm->active != 0) {
    Alarm_Withdraw_Arm7(&alarm->osAlarm);
    alarm->tag++;
    alarm->active = 0;
  }
}
