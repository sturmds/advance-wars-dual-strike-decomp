#include "types.h"
#include "game/sound_arc.h"

const SoundArcStrmInfo *NNS_SndArcGetStrmInfo(int strmNo);
BOOL SoundStream_Prepare(void **handle, const SoundArcStrmInfo *info, int slotNo, int priority,
                   int strmNo, u32 offset, int arg6, int arg7, int arg8, int arg9);

/* Prepares stream strmNo of the current sound archive on handle, starting
 * at offset. Returns FALSE when the archive has no such stream or it
 * cannot be set up. */
BOOL SoundArc_PrepareStream(void **handle, int strmNo, u32 offset)
{
    const SoundArcStrmInfo *info = NNS_SndArcGetStrmInfo(strmNo);

    if (info == NULL) {
        return FALSE;
    }
    return SoundStream_Prepare(handle, info, info->slotNo, info->priority, strmNo, offset, 0, 0, 0, 0);
}
