#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

const u32 *func_02106614(int seqArcNo);                /* sequence archive entry; first word is the offset of its data */
const void *func_02106304(u32 offset);
const SoundArcSeqInfo *func_0210924c(const void *seqArc, int index);
BOOL func_0210783c(SoundHandle *handle, int playerNo, int bankNo, int playerPrio,
                   const SoundArcSeqInfo *info, const void *seqArc, int seqArcNo, int index);

/* NitroSystem NNS_SndArcPlayerStartSeqArc: starts sequence number index of
 * sequence archive seqArcNo on handle. Returns TRUE if it started. */
BOOL NNS_SndArcPlayerStartSeqArc(SoundHandle *handle, int seqArcNo, int index)
{
    const u32 *entry;
    const void *seqArc;
    const SoundArcSeqInfo *info;

    entry = func_02106614(seqArcNo);
    if (entry == NULL) {
        return FALSE;
    }
    seqArc = func_02106304(*entry);
    if (seqArc == NULL) {
        return FALSE;
    }
    info = func_0210924c(seqArc, index);
    if (info == NULL) {
        return FALSE;
    }
    return func_0210783c(handle, info->playerNo, info->bankNo, info->playerPrio,
                         info, seqArc, seqArcNo, index);
}
