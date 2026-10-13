#include "types.h"

/* also declared in src/game/main/sound/sound_lock_channels.c and 1 more */
int gLockedSoundChannels;
int data_022a8044; /* also declared in src/game/main/sound/func_02104f90.c */
/* also declared in src/game/main/sound/sound_alarm_alloc.c and 1 more */
int data_022a8048;

void* func_02104f00(void) {
  gLockedSoundChannels = 0;
  data_022a8044 = 0;
  data_022a8048 = 0;
  return &data_022a8048;
}
