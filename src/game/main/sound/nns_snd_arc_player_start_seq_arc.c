#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

const u32 *NNS_SndArcGetSeqArcInfo(int seqArcNo);                /* sequence archive entry; first word is the offset of its data */
const SoundSeqArcFile *SoundArc_GetFileAddress(u32 offset);
const SoundArcSeqInfo *SoundSeqArc_GetSeqInfo(const SoundSeqArcFile *seqArc, int index);
BOOL SoundHandle_StartSeqArcSeq(SoundHandle *handle, int playerNo, int bankNo, int playerPrio,
                   const SoundArcSeqInfo *info, const SoundSeqArcFile *seqArc, int seqArcNo, int index);

/* NitroSystem NNS_SndArcPlayerStartSeqArc: starts sequence number index of
 * sequence archive seqArcNo on handle. Returns TRUE if it started. */
BOOL NNS_SndArcPlayerStartSeqArc(SoundHandle *handle, int seqArcNo, int index)
{
    const u32 *entry;
    const SoundSeqArcFile *seqArc;
    const SoundArcSeqInfo *info;

    entry = NNS_SndArcGetSeqArcInfo(seqArcNo);
    if (entry == NULL) {
        return FALSE;
    }
    seqArc = SoundArc_GetFileAddress(*entry);
    if (seqArc == NULL) {
        return FALSE;
    }
    info = SoundSeqArc_GetSeqInfo(seqArc, index);
    if (info == NULL) {
        return FALSE;
    }
    return SoundHandle_StartSeqArcSeq(handle, info->playerNo, info->bankNo, info->playerPrio,
                         info, seqArc, seqArcNo, index);
}
