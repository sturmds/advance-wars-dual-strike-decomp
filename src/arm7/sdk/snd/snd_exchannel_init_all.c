#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void SndExChannel_InitAll(void) {
  int i;
  SndExChannel* channel;

  for (i = 0; i < AUDIO_CH_TOTAL; i++) {
    channel = &data_arm7_03808090.channels[i];
    channel->id = i;
    channel->sync = 0;
    channel->active = FALSE;
  }
  data_arm7_0380806c = 0;
  data_arm7_03808068 = 0;
}
