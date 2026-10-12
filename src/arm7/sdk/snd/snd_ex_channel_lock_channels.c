#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void func_arm7_037ff68c(SndExChannel* channel);

/* Stops the channels in channelMask that are not locked yet, then adds them to the lock or weak lock. */
void SndExChannel_LockChannels(u32 channelMask, u32 flags) {
  SndExChannel* channel;
  u32 bits;
  int i;

  bits = channelMask;
  for (i = 0; i < AUDIO_CH_TOTAL && bits != 0; i++, bits >>= 1) {
    if (!(bits & 1)) {
      continue;
    }
    channel = &data_arm7_03808090.channels[i];
    if (data_arm7_0380806c & (1 << i)) {
      continue;
    }
    if (channel->callback != NULL) {
      channel->callback(channel, VOICE_STOLEN, channel->callbackArg);
    }
    SndHw_StopChannel(i, 0);
    channel->priority = 0;
    func_arm7_037ff68c(channel);
    channel->sync = 0;
    channel->active = FALSE;
  }
  if (flags & VOICE_RESERVE_SOFT) {
    data_arm7_03808068 |= channelMask;
  } else {
    data_arm7_0380806c |= channelMask;
  }
}
