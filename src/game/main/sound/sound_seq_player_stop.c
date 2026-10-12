#include "types.h"
#include "game/sound.h"

void SoundFader_Start(SoundFader* fader, int target, int frames);
void SoundSeqPlayer_SetPriority(SoundSeqPlayer* player, int arg);
void SoundSeqPlayer_StopNow(SoundSeqPlayer* player);

/* Stops a sequence player. With no fade frames it stops at once; otherwise it fades the volume to 0
 * and the player stops when the fade ends. */
void SoundSeqPlayer_Stop(SoundSeqPlayer* player, int fadeFrames) {
  if (player == NULL) {
    return;
  }
  if (player->state == SEQ_PLAYER_FREE) {
    return;
  }
  if (fadeFrames == 0) {
    SoundSeqPlayer_StopNow(player);
    return;
  }
  SoundFader_Start(&player->volumeFader, 0, fadeFrames);
  SoundSeqPlayer_SetPriority(player, 0);
  player->state = SEQ_PLAYER_STOPPING;
}
