#include "sdk/snd.h"

void func_arm7_037fe848(int channel, int timer);

void SndArm7_SetChannelTimer(u32 channelMask, int timer) {
  int channel;

  for (channel = 0; channel < 16 && channelMask != 0;
       channel++, channelMask >>= 1) {
    if (channelMask & 1) {
      func_arm7_037fe848(channel, timer);
    }
  }
}
