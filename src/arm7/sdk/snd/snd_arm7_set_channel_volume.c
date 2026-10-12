#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void SndArm7_SetChannelVolume(u32 channelMask, int volume, int shift) {
  int channel;

  for (channel = 0; channel < 16 && channelMask != 0;
       channel++, channelMask >>= 1) {
    if (channelMask & 1) {
      SndHw_SetChannelVolume(channel, volume, shift);
    }
  }
}
