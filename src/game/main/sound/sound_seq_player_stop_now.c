#include "types.h"
#include "game/sound_handle.h"

void func_0210fe48(int hwChannel, int volume);
void SND_StopSeq(int hwChannel);
void SoundSeqPlayer_Release(SoundSeqPlayer* player);

/* Stops a sequence player at once and returns it to the free list. A player
 * that was fading out is first set to the lowest volume so it cuts off
 * silently. */
void SoundSeqPlayer_StopNow(SoundSeqPlayer* player) {
  if (player->state == SEQ_PLAYER_STOPPING) {
    func_0210fe48(player->hwChannel, -723);
  }
  SND_StopSeq(player->hwChannel);
  SoundSeqPlayer_Release(player);
}
