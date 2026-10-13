#include "types.h"
#include "game/sound.h"

u32 gLockedSoundChannels;
u32 data_022a8044;
u32 data_022a8048;

void* func_02104f00(void) {
  gLockedSoundChannels = 0;
  data_022a8044 = 0;
  data_022a8048 = 0;
  return &data_022a8048;
}
