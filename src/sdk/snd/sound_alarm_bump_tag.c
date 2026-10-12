#include "sdk/snd.h"

extern SndAlarm data_022ab740[SND_ALARM_COUNT];

void SoundAlarm_BumpTag(int alarmNo) {
  SndAlarm* alarm = &data_022ab740[alarmNo];

  alarm->tag++;
}
