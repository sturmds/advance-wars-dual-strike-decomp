#include "sdk/snd.h"

extern SndAlarm data_022ab740[SND_ALARM_COUNT];

void SoundAlarm_RunIfTagMatches(int alarmId) {
  SndAlarm* alarm = &data_022ab740[alarmId & 0xff];

  if (((alarmId >> 8) & 0xff) != alarm->tag) {
    return;
  }
  if (alarm->handler == NULL) {
    return;
  }
  alarm->handler(alarm->arg);
}
