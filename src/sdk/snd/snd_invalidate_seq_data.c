#include "sdk/snd.h"

void SND_InvalidateSeqData(u32 start, u32 end) {
  Sound_PostCommandArgs(28, start, end, 0, 0);
}
