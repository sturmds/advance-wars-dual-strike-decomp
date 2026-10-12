#include "sdk/snd_seq.h"
#include "sdk/snd.h"

/* Stops every playing sequence whose bank lies in [start, end]. */
void SndArm7_InvalidateBank(u32 start, u32 end) {
  int i;

  for (i = 0; i < SND_PLAYER_COUNT; i++) {
    SndPlayer* player = &data_arm7_038085d0[i];

    if (player->active && start <= (u32)player->bank &&
        (u32)player->bank <= end) {
      SndPlayer_Stop(player);
    }
  }
}
