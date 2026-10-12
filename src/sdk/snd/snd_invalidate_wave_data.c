#include "sdk/snd.h"

void SND_InvalidateWaveData(u32 start, u32 end) {
  Sound_PostCommandArgs(30, start, end, 0, 0);
}
