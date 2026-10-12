#include "sdk/snd.h"

void SndPlayer_Stop(SndPlayer* player) {
  int i;

  for (i = 0; i < SND_TRACK_COUNT; i++) {
    SndPlayer_CloseTrack(player, i);
  }
  player->active = FALSE;
}
