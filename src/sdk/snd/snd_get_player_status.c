#include "sdk/snd.h"
#include "sdk/os.h"

extern SndSharedWork* data_022ab7a0;

u32 SND_GetPlayerStatus(void) {
  DC_InvalidateRange(data_022ab7a0, sizeof(SndSharedWork));
  return data_022ab7a0->playerStatus;
}
