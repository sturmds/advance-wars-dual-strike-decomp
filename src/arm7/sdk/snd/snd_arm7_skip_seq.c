#include "sdk/snd.h"
#include "sdk/snd_seq.h"

SndSharedWork* data_arm7_0380808c;
SndArm7Work data_arm7_03808090;

/* Moves the player's sequence ticks ticks forward without playing it: releases
 * and frees every channel first, then steps with the interval timer stopped. */
void SndArm7_SkipSeq(int playerNo, u32 ticks) {
  SndPlayer* player = &data_arm7_038085d0[playerNo];
  int i;
  u32 tick;

  for (i = 0; i < SND_TRACK_COUNT; i++) {
    SndTrack* track = SndPlayer_GetTrack(player, i);
    if (track != NULL) {
      SndTrack_ReleaseChannels(track, player, 127);
      SndTrack_FreeChannels(track);
    }
  }
  SND_StopIntervalTimer();
  for (tick = 0; tick < ticks; tick++) {
    if (SndPlayer_StepTracks(player, FALSE)) {
      SndPlayer_Stop(player);
      break;
    }
  }
  SND_StartIntervalTimer();
  if (data_arm7_0380808c != NULL) {
    data_arm7_0380808c->player[player->number].tickCounter += tick;
  }
}
