#include "types.h"
#include "game/sound_strm.h"

void func_0210fc5c(u32 channelMask, int pan);

/* Sets the pan (0 left, 127 right) of one channel of the stream. Does
 * nothing when the stream has no such channel. */
void SoundStrm_SetPan(SoundStrm* strm, int index, int pan) {
  if (index > strm->channelCount - 1) {
    return;
  }
  func_0210fc5c(1 << strm->channels[index], pan);
}
