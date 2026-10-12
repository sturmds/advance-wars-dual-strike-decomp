#include "types.h"

extern int gLockedSoundChannels;
extern int data_022a8044;
extern int data_022a8048;

void* func_02104f00(void) {
  gLockedSoundChannels = 0;
  data_022a8044 = 0;
  data_022a8048 = 0;
  return &data_022a8048;
}
