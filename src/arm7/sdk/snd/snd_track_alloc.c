#include "sdk/snd.h"

/* Returns the index of a newly claimed track, or -1 when all are in use. */
int SndTrack_Alloc(void) {
  int i;

  for (i = 0; i < SEQ_TRACKS_SHARED_TOTAL; i++) {
    SndTrack* track = &data_arm7_03808810[i];
    if (!track->active) {
      track->active = TRUE;
      return i;
    }
  }
  return -1;
}
