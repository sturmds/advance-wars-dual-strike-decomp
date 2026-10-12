#include "sdk/snd.h"
#include "sdk/os.h"

extern SndSharedWork* data_022ab7a0;

u16 SND_GetChannelStatus(void) {
  DC_InvalidateRange(data_022ab7a0, sizeof(SndSharedWork));
  return data_022ab7a0->channelStatus;
}
