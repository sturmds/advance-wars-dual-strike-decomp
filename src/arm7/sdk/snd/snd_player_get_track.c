#include "sdk/snd.h"

SndTrack* SndPlayer_GetTrack(SndPlayer* player, int trackNo) {
  /* needed to match: the original compares with 15, not 16 */
  if (trackNo > SND_TRACK_COUNT - 1) {
    return NULL;
  }
  if (player->tracks[trackNo] == SEQ_TRACK_SLOT_EMPTY) {
    return NULL;
  }
  return &data_arm7_03808810[player->tracks[trackNo]];
}
