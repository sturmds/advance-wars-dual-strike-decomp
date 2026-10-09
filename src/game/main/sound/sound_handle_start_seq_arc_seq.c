#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

SoundSeqPlayer *func_02105488(SoundHandle *handle, int playerNo, int playerPrio);
void *func_02105344(int playerNo, SoundSeqPlayer *player);
int SoundArc_GetBank(int bankNo, int loadFlags, void *heap, int cache, void **loadedOut);
void func_0210547c(SoundSeqPlayer *player);
void func_02105400(SoundSeqPlayer *player, const void *seqData, u32 offset, void *bank);
void func_02105814(SoundHandle *handle, int volume);
void func_0210579c(SoundHandle *handle, int channelPrio);
void func_02105758(SoundHandle *handle, int seqArcNo, int index);

/* Starts sequence index of sequence archive seqArcNo on handle: takes a
 * player, loads the bank into its heap, then hands it the sequence data.
 * Returns FALSE when no player is free or the bank does not load. */
BOOL SoundHandle_StartSeqArcSeq(SoundHandle *handle, int playerNo, int bankNo, int playerPrio,
                   const SoundArcSeqInfo *info, const SoundSeqArcFile *seqArc, int seqArcNo, int index)
{
    SoundSeqPlayer *player;
    void *bank;

    player = func_02105488(handle, playerNo, playerPrio);
    if (player == NULL) {
        return FALSE;
    }
    if (SoundArc_GetBank(bankNo, 6, func_02105344(playerNo, player), 0, &bank) != 0) {
        func_0210547c(player);
        return FALSE;
    }
    func_02105400(player, (const u8 *)seqArc + seqArc->dataOffset, info->fileId, bank);
    func_02105814(handle, info->volume);
    func_0210579c(handle, info->channelPrio);
    func_02105758(handle, seqArcNo, index);
    return TRUE;
}
