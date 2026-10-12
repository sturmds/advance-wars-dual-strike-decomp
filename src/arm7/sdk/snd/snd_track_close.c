#include "types.h"
#include "sdk/snd_seq.h"

/* Releases the track's channels at once and frees them. */
void SndTrack_Close(SndTrack* track, SndPlayer* player) {
  SndTrack_ReleaseChannels(track, player, -1);
  SndTrack_FreeChannels(track);
}
