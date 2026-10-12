#include "types.h"
#include "game/sound.h"
#include "game/sound_handle.h"

#define SEQ_PLAYER_COUNT 16

extern SoundSeqPlayer
    gSoundSeqPlayers[SEQ_PLAYER_COUNT]; /* NitroSystem sequence players */

/* NitroSystem NNS_SndPlayerStopSeqAll: stops every sequence player that is in use. */
void NNS_SndPlayerStopSeqAll(int fadeFrames) {
  int i;

  for (i = 0; i < SEQ_PLAYER_COUNT; i++) {
    if (gSoundSeqPlayers[i].state != SEQ_PLAYER_FREE) {
      SoundSeqPlayer_Stop(&gSoundSeqPlayers[i], fadeFrames);
    }
  }
}
