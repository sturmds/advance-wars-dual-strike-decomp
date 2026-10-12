#include "sdk/snd_hw.h"

BOOL SndHw_IsChannelBusy(int channel) {
  return (AUDIO_CH_REG_BLOCKS[channel].flags & AUDIO_CH_PLAYING_FLAG) != 0;
}
