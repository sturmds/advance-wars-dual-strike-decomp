#include "sdk/snd_seq.h"

u32 SndExChannel_GetLockedChannels(u32 flags) {
  if (flags & VOICE_RESERVE_SOFT) {
    return data_arm7_03808068;
  } else {
    return data_arm7_0380806c;
  }
}
