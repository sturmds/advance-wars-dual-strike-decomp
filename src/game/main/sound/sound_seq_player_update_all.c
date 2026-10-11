#include "types.h"
#include "game/sound_handle.h"

extern LinkList data_022a805c;             /* all sequence players, sorted by priority */
extern s16 data_02151534[];                /* volume table */

/* Lowest volume value the hardware channels take. */
#define SOUND_DECIBEL_FLOOR (-723)

u32 func_02110968(void);
BOOL SND_IsFinishedCommandTag(u32 tick);
void SoundSeqPlayer_Release(SoundSeqPlayer *player);
void func_0210fe48(int hwChannel, int volume);
BOOL func_0210928c(SoundFader *fader);
void func_021092a4(SoundFader *fader);
int SoundFader_GetValue(SoundFader *fader);
void SoundSeqPlayer_StopNow(SoundSeqPlayer *player);

/* Per-frame step for every sequence player: frees players whose hardware
 * channel went quiet, otherwise sets the channel volume to the sum of the
 * group, sequence, player and fader volumes, and stops a player whose
 * closing fade has ended. */
void SoundSeqPlayer_UpdateAll(void)
{
    u32 activeChannels = func_02110968();
    SoundSeqPlayer *player = LinkList_GetNext(&data_022a805c, NULL);
    SoundSeqPlayer *next;
    int zero = 0;                          /* needed to match: the original reads the 0 from a stack slot */
    int playerVolume;
    int seqVolume;
    int slotVolume;
    int volume;
    int fadeIndex;

    if (player == NULL) {
        return;
    }
    do {
        next = LinkList_GetNext(&data_022a805c, player);
        if (!player->started && SND_IsFinishedCommandTag(player->startTick)) {
            player->started = 1;
        }
        if (player->started && !(activeChannels & (1 << player->hwChannel))) {
            SoundSeqPlayer_Release(player);
        } else {
            func_021092a4(&player->volumeFader);
            playerVolume = data_02151534[player->volume];
            seqVolume = data_02151534[player->seqVolume];
            slotVolume = data_02151534[player->ownerSlot->volume];
            fadeIndex = SoundFader_GetValue(&player->volumeFader) >> 8;
            volume = data_02151534[fadeIndex] + (slotVolume + (seqVolume + playerVolume));
            if (volume < SOUND_DECIBEL_FLOOR) {
                volume = SOUND_DECIBEL_FLOOR;
            } else if (volume > 0) {
                volume = zero;
            }
            if (volume != player->hwVolume) {
                func_0210fe48(player->hwChannel, volume);
                player->hwVolume = volume;
            }
            if (player->state == SEQ_PLAYER_STOPPING && func_0210928c(&player->volumeFader)) {
                SoundSeqPlayer_StopNow(player);
            }
        }
        player = next;
    } while (player != NULL);
}
