#include "types.h"
#include "game/sound.h"

BOOL func_021076b0(int itemNo, int loadFlags, void *heap, int arg3, void *arg4);

/* Loads one sound archive item into the sound heap (probably
 * NNS_SndArcLoadWaveArc). TRUE on success. */
BOOL func_02107804(int itemNo, void *heap)
{
    return func_021076b0(itemNo, 0xff, heap, 1, NULL) == 0;
}
