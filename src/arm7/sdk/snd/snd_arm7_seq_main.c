#include "sdk/snd.h"
#include "sdk/snd_seq.h"

void func_arm7_03800ed0(SndPlayer *player);
void SndPlayer_UpdateChannels(SndPlayer *player);

/* stepSequence is FALSE on driver runs between sequence ticks: only the channels are updated then.
 * SndPlayer.prepared holds the pause flag (see SndArm7_PauseSeq). */
void SndArm7_SeqMain(BOOL stepSequence)
{
    SndPlayer *player;
    int i;
    u32 status = 0;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        player = &data_arm7_038085d0[i];
        if (player->active) {
            if (stepSequence && !player->prepared) {
                func_arm7_03800ed0(player);
            }
            SndPlayer_UpdateChannels(player);
            if (player->active) {
                status |= 1 << i;
            }
        }
    }
    if (data_arm7_0380808c != NULL) {
        data_arm7_0380808c->playerStatus = status;
    }
}
