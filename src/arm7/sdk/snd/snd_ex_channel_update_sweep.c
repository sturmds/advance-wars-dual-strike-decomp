#include "sdk/snd_seq.h"

/* The compiler's signed 64-bit divide (ARM7 copy of _ll_sdiv). The original calls it by this name. */
s64 _ll_sdiv_Arm7(s64 dividend, s64 divisor);

/*
 * Returns the channel's sweep pitch offset: sweepPitch at the start, falling
 * linearly to 0 after sweepLength steps. Advances one step when update is set
 * and the channel sweeps by itself.
 */
int SndExChannel_UpdateSweep(SndExChannel* channel, BOOL update) {
  s64 offset;

  if (channel->sweepPitch == 0) {
    return 0;
  }
  if (channel->sweepCounter >= channel->sweepLength) {
    return 0;
  }
  offset = _ll_sdiv_Arm7(
      (s64)channel->sweepPitch * (channel->sweepLength - channel->sweepCounter),
      channel->sweepLength);
  if (update && channel->autoSweep) {
    channel->sweepCounter++;
  }
  return offset;
}
