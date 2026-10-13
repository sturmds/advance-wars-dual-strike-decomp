#include "sdk/snd.h"

/* also declared in src/sdk/snd/sound_alarm_assign_handler.c and 2 more */
SndAlarm data_022ab740[SND_ALARM_COUNT];

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
