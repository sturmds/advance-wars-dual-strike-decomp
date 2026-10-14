#include "sdk/snd_hw.h"

int func_arm7_037fe63c(void);

void SndHw_Reset(void) {
  int channel;

  func_arm7_037fe63c();
  for (channel = 0; channel < SOUND_CHANNEL_COUNT; channel++) {
    SndHw_StopChannel(channel, 1);
  }
  AUDIO_RECORD0_CTRL = 0;
  AUDIO_RECORD1_CTRL = 0;
}
