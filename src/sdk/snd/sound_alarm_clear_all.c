#include "sdk/snd.h"

extern SndAlarm data_022ab740[SND_ALARM_COUNT];

void SoundAlarm_ClearAll(void) {
  int i;

  for (i = 0; i < SND_ALARM_COUNT; i++) {
    data_022ab740[i].handler = NULL;
    data_022ab740[i].arg = 0;
    data_022ab740[i].tag = 0;
  }
}
