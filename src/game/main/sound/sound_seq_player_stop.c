#include "types.h"
#include "game/sound.h"

void func_021092f8(SoundFader *fader, int target, int frames);
void func_02105038(SoundSeqPlayer *player, int arg);
void SoundSeqPlayer_StopNow(SoundSeqPlayer *player);

/* Stops a sequence player. With no fade frames it stops at once; otherwise it fades the volume to 0
 * and the player stops when the fade ends. */
void SoundSeqPlayer_Stop(SoundSeqPlayer *player, int fadeFrames)
{
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
    func_021092f8(&player->volumeFader, 0, fadeFrames);
    func_02105038(player, 0);
    player->state = SEQ_PLAYER_STOPPING;
}
