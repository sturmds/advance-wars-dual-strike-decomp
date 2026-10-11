#include "types.h"
#include "game/sound_arc.h"

/* Sequence number index of a sequence archive file, or NULL when index is
 * out of range or the slot is unused. */
const SoundArcSeqInfo *SoundSeqArc_GetSeqInfo(const SoundSeqArcFile *seqArc, int index)
{
    if (index < 0) {
        return NULL;
    }
    if (index >= seqArc->seqCount) {
        return NULL;
    }
    if (seqArc->seqs[index].fileId == 0xffffffff) {
        return NULL;
    }
    return &seqArc->seqs[index];
}
