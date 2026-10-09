#include "types.h"
#include "game/sound.h"

BOOL func_021076b0(int itemNo, int loadFlags, void *heap, int arg3, void *arg4);

/* Loads a sequence archive into the sound heap. TRUE on success.
 * func_021076b0 starts from NNS_SndArcGetSeqArcInfo. */
BOOL NNS_SndArcLoadSeqArc(int itemNo, void *heap)
{
    return func_021076b0(itemNo, 0xff, heap, 1, NULL) == 0;
}
