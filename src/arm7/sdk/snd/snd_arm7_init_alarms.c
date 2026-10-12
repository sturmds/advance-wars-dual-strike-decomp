#include "sdk/snd.h"

void SndArm7_InitAlarms(void) {
  int i;

  for (i = 0; i < SOUND_TIMER_SLOTS_ARM7; i++) {
    data_arm7_03808090.alarms[i].active = 0;
    data_arm7_03808090.alarms[i].tag = 0;
  }
}
