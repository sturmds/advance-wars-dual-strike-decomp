#include "types.h"
#include "game/sound.h"

void SoundFader_Start(SoundFader *fader, int target, int frames);

/* Fades the player's volume to targetVolume over frames frames. Does
 * nothing while the player is stopping. The fader holds volume in 8.8
 * fixed point. */
void NNS_SndPlayerMoveVolume(SoundHandle *handle, int targetVolume, int frames)
{
    SoundSeqPlayer *player = handle->player;

    if (player == NULL) {
        return;
    }
    if (player->state == SEQ_PLAYER_STOPPING) {
        return;
    }
    SoundFader_Start(&player->volumeFader, targetVolume << 8, frames);
}
