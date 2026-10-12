#include "sdk/snd.h"

void SND_SetupChannelPcm(u32 channel, u32 format, u32 data, u32 loop,
                         u32 loopStart, u32 loopLength, u32 volume,
                         u32 volumeShift, u32 timer, u32 pan) {
  Sound_PostCommandArgs(12, channel | timer << 16, data,
                        loopLength | (volume << 24 | volumeShift << 22),
                        loopStart | (loop << 26 | format << 24 | pan << 16));
}
